#include "FiberTrackingAlgorithmGeometryShader.h"

#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"
#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"
#include "utilities/QCheckingOpenGL.h"
#include <cmath>

#define BUFFER_OFFSET(x)  ((const void*) (x))

FiberTrackingAlgorithmGeometryShader::FiberTrackingAlgorithmGeometryShader(const DTI *dti):
    FiberTrackingAlgorithm(dti),
    m_vao(0),
    mFilteredSeedPositions(nullptr),
    mNumVerticesPerFiber(nullptr)
{
    mProgramGSFiberTracking = nullptr;
}

FiberTrackingAlgorithmGeometryShader::~FiberTrackingAlgorithmGeometryShader()
{
    if(mProgramGSFiberTracking != nullptr)
        delete mProgramGSFiberTracking;

    if(m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);
}

void FiberTrackingAlgorithmGeometryShader::initialize()
{
    GLint value;
    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &value);
    std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES " << value << std::endl;
    m_max_number_vertices_geometry_shader = value;

    glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &value);
    std::cout << "GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS " << value << std::endl;
    m_max_number_components_geometry_shader = value;

    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &value);
    std::cout << "GL_MAX_GEOMETRY_OUTPUT_COMPONENTS " << value << std::endl;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    initialize_seed();

    //#######################################################################################################################################
    QManagementResource *vs = new QManagementResource;
    QManagementResource *gs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, ":/diff_particles_vert");
    GLuint geometry = gs->compileResourceFileShader(QManagementResource::geometry, ":/diff_particles_geom");

    mProgramGSFiberTracking = new vmtkManagementShader;
    mProgramGSFiberTracking->setShader(vertex);
    mProgramGSFiberTracking->setShader(geometry);

    const char *varying_name_tracing[] = {"fiber_point"};
    glTransformFeedbackVaryings(mProgramGSFiberTracking->getProgram(), 1, varying_name_tracing, GL_INTERLEAVED_ATTRIBS);

    mProgramGSFiberTracking->linking();
    glLinkProgram(mProgramGSFiberTracking->getProgram());

    delete vs;
    delete gs;

    //---------------------------------------------------------------------------------------------------------------------------------------

    mProgramGSFiberTracking->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "w_punct"), getFiberTrackingWpunct());

        setFiberStepGeometryShader();

        mAttribLocSeedPosition = glGetAttribLocation(mProgramGSFiberTracking->getProgram(), "seed_position");
        mAttribLocDirection = glGetAttribLocation(mProgramGSFiberTracking->getProgram(), "direction");
        if(mAttribLocDirection < 0)
            std::cout << "mAttribLocDirection Negative..." << std::endl;

        glUniform3fv(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "phy_dimension"), 1, m_phyDim);

        createTensorImages();
    }
    mProgramGSFiberTracking->releasing();
}

std::shared_ptr<FiberClusterSet> FiberTrackingAlgorithmGeometryShader::startFiberTracking()
{
    if(mNumCreatedSeeds != 0)
    {
        glBindVertexArray(m_vao);

        filterSeeds();
        createFibers();

        glBindVertexArray(0);

        return mFiberClusterVector;
    }
    else
    {
        return std::make_shared<FiberClusterSet>();
    }
}

//Insertion Sort Implementetion
void FiberTrackingAlgorithmGeometryShader::orderSeedsBySizeInsertionSort(unsigned int array[], int size, unsigned int id[], FiberCluster::MyVec4 * index)
{
    int i, j;
    unsigned int key, id_key;
    FiberCluster::MyVec4 index_key;

    for(i = 1; i < size; i++)
    {
        key = array[i];
        id_key = id[i];
        index_key = index[i];
        j = i-1;
        /*Move elements of arr[0..i-1] that are  smaller than key */
        while(j>=0 && array[j] < key)
        {
            array[j+1] = array[j];
            id[j+1] = id[j];
            index[j+1] = index[j];

            j = j - 1;
        }
        array[j+1] = key;
        id[j+1] = id_key;
        index[j+1] = index_key;
    }
}

//Quicksort implementation with pivot as last element//
void FiberTrackingAlgorithmGeometryShader::orderSeedsBySize(unsigned int array[], int low, int high, unsigned int id[], FiberCluster::MyVec4 * index)
{
    if(low < high)
    {
        int p = partition(array, low, high, id, index);

        orderSeedsBySize(array, low, p-1, id, index);
        orderSeedsBySize(array, p + 1, high, id, index);
    }
}

int FiberTrackingAlgorithmGeometryShader::partition(unsigned int array[], int low, int high, unsigned int id[], FiberCluster::MyVec4 *index)
{
    unsigned int pivot = array[high];
    int i = low - 1;

    for(int j = low; j <= high-1; j++)
    {
        if(array[j] >= pivot)
        {
            i++;
            swap(&array[i], &array[j]);
            swap(&id[i], &id[j]);
            swap(&index[i], &index[j]);
        }
    }
    swap(&array[i+1], &array[high]);
    swap(&id[i+1], &id[high]);
    swap(&index[i+1], &index[high]);
    return (i+1);
}

void FiberTrackingAlgorithmGeometryShader::swap(unsigned int * x, unsigned int * y)
{
    unsigned int aux = *x;
    *x = *y;
    *y = aux;
}

void FiberTrackingAlgorithmGeometryShader::swap(FiberCluster::MyVec4 * x, FiberCluster::MyVec4 * y)
{
    FiberCluster::MyVec4 aux = *x;
    *x = *y;
    *y = aux;
}

bool FiberTrackingAlgorithmGeometryShader::equalPoints(FiberCluster::MyVec4 a, FiberCluster::MyVec4 b)
{
    double epsilon = 0.000000001;
    return (std::fabs(a.x - b.x) < epsilon && std::fabs(a.y - b.y) < epsilon && std::fabs(a.z - b.z) < epsilon);
}

bool FiberTrackingAlgorithmGeometryShader::equalPoints(FiberCluster::MyVec3 a, FiberCluster::MyVec3 b)
{
    double epsilon = 0.000000001;
    return (std::fabs(a.x - b.x) < epsilon && std::fabs(a.y - b.y) < epsilon && std::fabs(a.z - b.z) < epsilon);
}

bool FiberTrackingAlgorithmGeometryShader::equalPoints(float ax, float ay, float az, float bx, float by, float bz)
{
    double epsilon = 0.000000001;
    return (std::fabs(ax - bx) < epsilon && std::fabs(ay - by) < epsilon && std::fabs(az - bz) < epsilon);
}

void FiberTrackingAlgorithmGeometryShader::computeLoops(unsigned int n, unsigned int * size, unsigned int * seeds_loops, bool debug)
{
    if(debug)
        std::cout << "Number of Loops"<< std::endl;

    for(unsigned int i = 0; i < n; i++)
    {
        if(size[i] >= 128)
        {
            if((size[i] - 128) % 127 > 0)
            {
                seeds_loops[i] = ((size[i] - 128)/127) + 1;
            }
            else
            {
                seeds_loops[i] = (size[i] - 128)/127;
            }
        }
        else
        {
            seeds_loops[i] = 0;
        }

        if(debug)
        {
            if(seeds_loops[i] > 0)
            {
                std::cout << seeds_loops[i] << " ";
            }

        }
    }
    if(debug)
        std::cout << std::endl;
}

void FiberTrackingAlgorithmGeometryShader::adjustSize(unsigned int n, unsigned int * seeds_loops, unsigned int num_seeds, unsigned int * original_size, unsigned int * iteration_size)
{
    for(unsigned int k = 0; k < num_seeds; k++)
    {
        if(seeds_loops[k] > 0)
        {
            if(seeds_loops[k] == n)
            {
                if((original_size[k] - 128) % 127 > 0)
                {
                    iteration_size[k] = (original_size[k]-128)%127 + 1;
                }
                else
                {
                    iteration_size[k] = 127 + 1;
                }
            }
            else
            {
                iteration_size[k] = 128;
            }
        }
        else
        {
            iteration_size[k] = original_size[k];
        }
    }
}

//TODO Compare and error proof this function code and replace on necessary code
unsigned int FiberTrackingAlgorithmGeometryShader::organizeRetrievedPoints(unsigned int n, unsigned int num_points, unsigned int num_valid_seeds,
                                                                           FiberCluster::MyVec4 * points, FiberCluster::MyVec4 * seeds,
                                                                           FiberCluster::MyVec4 * directions, unsigned int * iteration_size,
                                                                           std::vector<std::vector<FiberCluster::MyVec4>> &saved_vertices,
                                                                           std::vector<std::vector<FiberCluster::MyVec4>> &saved_directions,
                                                                           bool debug)
{

    //Use variable to skip first vertice on n > 0 iteration, that was last vertice of fiber in construction
    bool skip_vertice = false;
    unsigned int index_size = 0;
    int index_sum = 0;
    unsigned int updated_points = 0;
    unsigned int j;
    FiberCluster::MyVec4 end_point;
    end_point.x = -1.0;
    end_point.y = -1.0;
    end_point.z = -1.0;
    end_point.w = -1.0;


    for(j = 0; j < num_points; j = j + 2)
    {
        if(j==0 && n > 0)
            skip_vertice = true;
        if(j==0 && n == 0)
        {
            std::vector<FiberCluster::MyVec4> new_vec;
            saved_vertices.push_back(new_vec);
            saved_directions.push_back(new_vec);
        }

        index_sum++;

        //Every time a new set provenient from a single seed is found, this condition statement is true (except the first one)
        if(index_sum == 129 || equalPoints(end_point, points[j]) )
        {
            //Normal case, reach size and next point is the seed from next tract
            index_size++;
            index_sum = 1;

            //Make recording of first point skip, if not first iteration, due to replication of last point below
            if(n > 0)
                skip_vertice = true;

            if(equalPoints(end_point, points[j]))
            {
                skip_vertice = true;
                index_sum = 0;
            }

            if(n == 0)
            {
                std::vector<FiberCluster::MyVec4> new_vec;
                saved_vertices.push_back(new_vec);
                saved_directions.push_back(new_vec);
            }
        }

        if(index_sum == 128)
        {
            //Check if last group was closed with the maximal number of vertices allowed, if so, update the seed positions for next i loop
            if(iteration_size[index_size] == 128)
            {
                //Update Seed
                seeds[index_size].x = points[j].x;
                seeds[index_size].y = points[j].y;
                seeds[index_size].z = points[j].z;
                seeds[index_size].w = points[j].w;
                //Update Direction
                directions[index_size].x = points[j+1].x;
                directions[index_size].y = points[j+1].y;
                directions[index_size].z = points[j+1].z;
                directions[index_size].w = points[j+1].w;

                updated_points++;
            }
        }

        if(index_size == num_valid_seeds)
        {
            //std::cerr << "Found more Seeds than valid Seeds enabled (" << j << ")" << std::endl;
            skip_vertice = true;
            break;
        }

        if(!skip_vertice)
        {
            saved_vertices[index_size].push_back(points[j]);
            saved_directions[index_size].push_back(points[j+1]);
        }
        skip_vertice = false;
    }

    if(debug)
    {
        std::cout << "Organized Points: " << j << std::endl;
        std::cout << "Updated Seeds and Directions: " << updated_points << std::endl;
    }
    return saved_vertices.size();
}

unsigned int FiberTrackingAlgorithmGeometryShader::removeDuplicatePoints(std::vector<std::vector<FiberCluster::MyVec4>> &points)
{
    //Function to check if exist any duplicate points
    unsigned int removed_points_count = 0;
    unsigned int removed_points_tract = 0;
    unsigned long int max_i = points.size();
    for(unsigned long int i = 0; i < max_i; i++)
    {
        removed_points_tract = 0;
        int max_j = points[i].size();
        for (int j = 0; j < max_j-2; j++)
        {
            if(equalPoints(points[i][j], points[i][j+1]))
            {
                //remove duplicate
                points[i].erase(points[i].begin() + (j + 1));
                max_j = max_j - 1;
                j--;
                removed_points_tract++;
            }
        }
        removed_points_count+=removed_points_tract;
    }
    return removed_points_count;
}

void FiberTrackingAlgorithmGeometryShader::createFibers()
{
    mProgramGSFiberTracking->binding();
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);
        glUniform1i(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "tensorCoeffPart01"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart02);
        glUniform1i(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "tensorCoeffPart02"), 1);

        //Variable to control prints on console
        bool debug = false;

        if(debug)
        {
            std::cout << std::endl;
            std::cout << "****** Starting Fiber Computation on GS ******** " << std::endl;
            std::cout << std::endl;
        }

        //Iterate over each cluster of fiber
        for(unsigned short i = 0; i < (*mFiberClusterVector).size(); i++)
        {
            unsigned int num_seed_positions = (*mFiberClusterVector)[i].getNumSeeds();
            unsigned int num_remaining_fiber_segments_points = (*mFiberClusterVector)[i].getNumFiberSegmentPoints();

            if(debug)
            {
                std::cout << "Cluster Number " << i << std::endl;
                std::cout << "Seed Positions: " << num_seed_positions << std::endl;
                std::cout << "Fiber Segments Points: " << num_remaining_fiber_segments_points << std::endl;
                std::cout << std::endl;
            }

            FiberCluster::MyVec4 *seed_positions = (*mFiberClusterVector)[i].getSeedPositions();

            unsigned int num_seed_positions_half = num_seed_positions/2;


            //Use to store Seed Position from each half
            FiberCluster::MyVec4 *seed_positions_orig = new FiberCluster::MyVec4[num_seed_positions_half];
            FiberCluster::MyVec4 *seed_positions_inv = new FiberCluster::MyVec4[num_seed_positions_half];
            //Store sizes of fibers for each half
            unsigned int * fiber_size_orig = new unsigned int[num_seed_positions_half];
            unsigned int * fiber_size_inv = new unsigned int[num_seed_positions_half];
            //Use as id's for reconnceting both halfs after sorting
            unsigned int * fiber_size_orig_id = new unsigned int[num_seed_positions_half];
            unsigned int * fiber_size_inv_id = new unsigned int[num_seed_positions_half];
            //Use to store directions between GS iterations
            FiberCluster::MyVec4 *directions_orig = new FiberCluster::MyVec4[num_seed_positions_half];
            FiberCluster::MyVec4 *directions_inv = new FiberCluster::MyVec4[num_seed_positions_half];
            //Store number of necessary loops for each half
            unsigned int * seeds_loops_orig = new unsigned int[num_seed_positions_half];
            unsigned int * seeds_loops_inv = new unsigned int[num_seed_positions_half];

            unsigned int index = 0;
            unsigned int sum_vertices_orig = 0;
            unsigned int sum_vertices_inv = 0;

            for(unsigned int j = 0; j < num_seed_positions; j+=2)
            {
                //First Half
                fiber_size_orig[index] = (*mFiberClusterVector)[i].getNumVerticesPerFiber()[j];
                seed_positions_orig[index] = seed_positions[j];
                sum_vertices_orig += (*mFiberClusterVector)[i].getNumVerticesPerFiber()[j];

                //Second Half
                fiber_size_inv[index] = (*mFiberClusterVector)[i].getNumVerticesPerFiber()[j+1];
                seed_positions_inv[index] = seed_positions[j+1];
                sum_vertices_inv += (*mFiberClusterVector)[i].getNumVerticesPerFiber()[j+1];

                //Set Initial Directions to a high Value
                directions_orig[index].x = 50.0;
                directions_orig[index].y = 50.0;
                directions_orig[index].z = 50.0;
                directions_orig[index].w = 50.0;

                directions_inv[index].x = 50.0;
                directions_inv[index].y = 50.0;
                directions_inv[index].z = 50.0;
                directions_inv[index].w = 50.0;

                //Set id as unsigned integers
                fiber_size_orig_id[index] = index;
                fiber_size_inv_id[index] = index;
                index++;
            }

            if(debug)
            {
                std::cout << "Total Fiber Points (Orig): " << sum_vertices_orig << std::endl;
                std::cout << "Total Fiber Points (Inv) : " << sum_vertices_inv << std::endl;
                std::cout << "Total (Orig + Inv)       : " << sum_vertices_inv + sum_vertices_orig << std::endl;
                std::cout << std::endl;
            }

            unsigned int num_seed_half_u = num_seed_positions_half;     //Cast from unsigned to signed on the belief that number of seeds won't exceed 2.147.483.647, that is the maximal of signed int value
            int num_seed_half = static_cast<int>(num_seed_half_u);

            //Order seed_positions acordingly to size
            orderSeedsBySizeInsertionSort(fiber_size_orig, num_seed_half, fiber_size_orig_id, seed_positions_orig);
            orderSeedsBySizeInsertionSort(fiber_size_inv, num_seed_half, fiber_size_inv_id, seed_positions_inv);

            //Compute number of necessary GS loops for each seed in each half
            computeLoops(num_seed_positions_half, fiber_size_orig, seeds_loops_orig, false);
            computeLoops(num_seed_positions_half, fiber_size_inv, seeds_loops_inv, false);

            if (debug)
            {
                std::cout << std::endl;
            }

            //Set Buffers and get results
            GLuint seedsVBO;
            GLuint verticesVBO;

            glGenBuffers(1, &seedsVBO);
            glGenBuffers(1, &verticesVBO);

            std::vector<std::vector<FiberCluster::MyVec4>> retrieved_vertices_orig;
            std::vector<std::vector<FiberCluster::MyVec4>> retrieved_directions_orig;
            float * input_orig = new float[8 * num_seed_positions_half];

            //Original Seed Group Loop
            for(unsigned int n = 0; n <= seeds_loops_orig[0]; n++)
            {
                //Adjust Seeds
                unsigned int num_valid_seeds = 0;       //Represents how many seeds still left for processing
                for(unsigned int k = 0; k < num_seed_positions_half; k++)
                {
                    if(seeds_loops_orig[k] >= n)             //Check if seed was already completed
                        num_valid_seeds++;              //If not, add as a valid seed
                }

                if (debug)
                {
                    std::cout << std::endl;
                    std::cout << "--------------------------------------------------------------------------------" << std::endl;
                    std::cout << std::endl;
                    std::cout << "Total number of valid Seeds is  " << num_valid_seeds << " on iteration " << n << std::endl;
                    std::cout << std::endl;
                }

                //Adjust Sizes
                unsigned int * fiber_size_orig_iter = new unsigned int[num_seed_positions_half];
                adjustSize(n, seeds_loops_orig, num_seed_positions_half, fiber_size_orig, fiber_size_orig_iter);

                //Create Input Vector based on valid seeds
                for(unsigned int lm = 0; lm < num_valid_seeds; lm++)
                {
                    //Seeds
                    input_orig[8*lm] = seed_positions_orig[lm].x;
                    input_orig[(8*lm)+1] = seed_positions_orig[lm].y;
                    input_orig[(8*lm)+2] = seed_positions_orig[lm].z;
                    input_orig[(8*lm)+3] = seed_positions_orig[lm].w;
                    //Directions
                    input_orig[(8*lm)+4] = directions_orig[lm].x;
                    input_orig[(8*lm)+5]  = directions_orig[lm].y;
                    input_orig[(8*lm)+6] = directions_orig[lm].z;
                    input_orig[(8*lm)+7] = directions_orig[lm].z;
                }

                //Set Buffers
                glBindBuffer(GL_ARRAY_BUFFER, seedsVBO);
                {
                    glBufferData(GL_ARRAY_BUFFER, num_valid_seeds * sizeof(GLfloat) * 8, input_orig, GL_STATIC_DRAW);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
                {
                    glBufferData(GL_ARRAY_BUFFER, 256 * num_valid_seeds * sizeof(GLfloat) * 4, nullptr, GL_STATIC_READ);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                FiberCluster::MyVec4 * vert_retrieve = new FiberCluster::MyVec4[num_valid_seeds*256];

                //Issue Geometry Shader
                unsigned int points_retrieved = trackFibers(verticesVBO, seedsVBO, num_valid_seeds, vert_retrieve, false);

                unsigned int sum_aux = 0;
                for(unsigned int hl = 0; hl < num_valid_seeds; hl++)
                {
                    sum_aux += fiber_size_orig_iter[hl];
                    if(seeds_loops_orig[hl] == n && (fiber_size_orig[hl]-128)%127 > 0)
                        sum_aux++;
                }

                if(debug)
                {
                    std::cout << "Points Retrieved (Orig): " << points_retrieved << std::endl;
                    std::cout << "Points Expected (Orig) : " << sum_aux*2 << std::endl;
                }

                //Organize retrieved Points
                organizeRetrievedPoints(n, points_retrieved, num_valid_seeds, vert_retrieve, seed_positions_orig, directions_orig, fiber_size_orig_iter, retrieved_vertices_orig, retrieved_directions_orig, debug);

                delete[] fiber_size_orig_iter;
                delete[] vert_retrieve;
            }


            /* ########################################################### ------ INV ------ ###############################################################################*/

            if(debug)
            {
                std::cout << std::endl;
                std::cout << std::endl;
                std::cout << "*************************************** Inv Seeds ********************************************************** " << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
            }

            std::vector<std::vector<FiberCluster::MyVec4>> retrieved_vertices_inv;
            std::vector<std::vector<FiberCluster::MyVec4>> retrieved_directions_inv;
            float * input_inv =  new float[8 * num_seed_positions_half];

            //Inverse Seed Group Loop
            for(unsigned int n = 0; n <= seeds_loops_inv[0]; n++)
            {
                //Adjust Seeds
                unsigned int num_valid_seeds_inv = 0;       //Represents how many seeds still left for processing
                for(unsigned int k = 0; k < num_seed_positions_half; k++)
                {
                    if(seeds_loops_inv[k] >= n)             //Check if seed was already completed
                        num_valid_seeds_inv++;              //If not, add as a valid seed
                }

                if (debug)
                {
                    std::cout << std::endl;
                    std::cout << "--------------------------------------------------------------------------------" << std::endl;
                    std::cout << std::endl;
                    std::cout << "Total number of valid Seeds is  " << num_valid_seeds_inv << " on iteration " << n << std::endl;
                    std::cout << std::endl;
                }

                //Adjust Sizes
                unsigned int * fiber_size_inv_iter = new unsigned int[num_seed_positions_half];
                adjustSize(n, seeds_loops_inv, num_seed_positions_half, fiber_size_inv, fiber_size_inv_iter);

                //Create Input Vector based on valid seeds
                for(unsigned int lm = 0; lm < num_valid_seeds_inv; lm++)
                {
                    //Seeds
                    input_inv[8*lm] = seed_positions_inv[lm].x;
                    input_inv[(8*lm)+1] = seed_positions_inv[lm].y;
                    input_inv[(8*lm)+2] = seed_positions_inv[lm].z;
                    input_inv[(8*lm)+3] = seed_positions_inv[lm].w;
                    //Directions
                    input_inv[(8*lm)+4] = directions_inv[lm].x;
                    input_inv[(8*lm)+5] = directions_inv[lm].y;
                    input_inv[(8*lm)+6] = directions_inv[lm].z;
                    input_inv[(8*lm)+7] = directions_inv[lm].w;
                }

                //Set Buffers
                glBindBuffer(GL_ARRAY_BUFFER, seedsVBO);
                {
                    glBufferData(GL_ARRAY_BUFFER, num_valid_seeds_inv * sizeof(GLfloat) * 8, input_inv, GL_STATIC_DRAW);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
                {
                    glBufferData(GL_ARRAY_BUFFER, 256 * num_valid_seeds_inv * sizeof(GLfloat) * 4, nullptr, GL_STATIC_READ);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                FiberCluster::MyVec4 * vert_retrieve = new FiberCluster::MyVec4[num_valid_seeds_inv*256];

                //Issue Geometry Shader
                unsigned int points_retrieved_inv = trackFibers(verticesVBO, seedsVBO, num_valid_seeds_inv, vert_retrieve, false);

                //Organize Retrieved Points
                organizeRetrievedPoints(n, points_retrieved_inv, num_valid_seeds_inv, vert_retrieve, seed_positions_inv, directions_inv, fiber_size_inv_iter, retrieved_vertices_inv, retrieved_directions_inv, debug);

                unsigned int sum_aux = 0;
                for(unsigned int hl = 0; hl < num_valid_seeds_inv; hl++)
                {
                    sum_aux += fiber_size_inv_iter[hl];
                    if(seeds_loops_inv[hl] == n && (fiber_size_inv[hl]-128)%127 > 0)
                        sum_aux++;
                }

                if(debug)
                {
                    std::cout << "Points Retrieved (Inv): " << points_retrieved_inv << std::endl;
                    std::cout << "Points Expected (Inv) : " << sum_aux*2 << std::endl;
                }

                delete[] fiber_size_inv_iter;
                delete[] vert_retrieve;
            }

            //------------------------------------------ Compile Final Result ------------------------------------------------------------------------------

            //Remove Duplicate Points
            unsigned int duplicate_orig = removeDuplicatePoints(retrieved_vertices_orig);
            unsigned int duplicate_directions_points_orig = removeDuplicatePoints(retrieved_directions_orig);
            unsigned int duplicate_inv = removeDuplicatePoints(retrieved_vertices_inv);
            unsigned int duplicate_directions_points_inv = removeDuplicatePoints(retrieved_directions_inv);

            if(debug)
            {
                std::cout << "Removed duplicated points (Orig)   : " << duplicate_orig << std::endl;
                std::cout << "Removed duplicated direction (Orig): " << duplicate_directions_points_orig << std::endl;
                std::cout << "Removed duplicated points (Inv)    :  " << duplicate_inv << std::endl;
                std::cout << "Removed duplicated direction (Inv): " << duplicate_directions_points_inv << std::endl;
                std::cout << std::endl;
            }
            //Unite both Seeds, i. e., Orig and Inv and organize based on original order, based on ID's
            unsigned int processed_vertices_orig = 0;
            for (unsigned int km = 0; km < retrieved_vertices_orig.size(); km++)
            {
                processed_vertices_orig+= retrieved_vertices_orig[km].size();
            }

            if(debug)
            {
                std::cout << "Number of Orig vertices processed: " << processed_vertices_orig << std::endl;
            }
            unsigned int processed_vertices_inv = 0;
            for (unsigned int km = 0; km < retrieved_vertices_inv.size(); km++)
            {
                processed_vertices_inv+= retrieved_vertices_inv[km].size();
            }

            unsigned int processed_vertices = processed_vertices_inv + processed_vertices_orig;

            if(debug)
            {
                std::cout << "Number of Inv vertices processed:  " << processed_vertices_inv << std::endl;
                std::cout << "Total:                             " << processed_vertices << std::endl;
            }

            FiberCluster::MyVec4 * vertices_result = new FiberCluster::MyVec4[processed_vertices];
            FiberCluster::MyVec4 * directions_result = new FiberCluster::MyVec4[processed_vertices];
            unsigned int * num_vert_per_fiber = new unsigned int[num_seed_positions];

            unsigned int ind_size_orig;
            unsigned int ind_size_inv;
            unsigned int index_total = 0;

            for(unsigned int j = 0; j < num_seed_positions_half; j++)
            {
                ind_size_orig = 0;
                for (unsigned int m = 0; m < num_seed_positions_half; m++)
                {
                    if(fiber_size_orig_id[m] == j)
                        break;
                    ind_size_orig++;
                }

                num_vert_per_fiber[2*j] = retrieved_vertices_orig[ind_size_orig].size();


                for (unsigned int l = 0; l < retrieved_vertices_orig[ind_size_orig].size(); l++)
                {
                    vertices_result[index_total] = retrieved_vertices_orig[ind_size_orig][l];
                    directions_result[index_total] = retrieved_directions_orig[ind_size_orig][l];
                    index_total++;
                }


                ind_size_inv = 0;
                for (unsigned int m = 0; m < num_seed_positions_half; m++)
                {
                    if(fiber_size_inv_id[m] == j)
                        break;
                    ind_size_inv++;
                }

                num_vert_per_fiber[(2*j) + 1] = retrieved_vertices_inv[ind_size_inv].size();

                for (unsigned int l = 0; l < retrieved_vertices_inv[ind_size_inv].size(); l++)
                {
                    vertices_result[index_total] = retrieved_vertices_inv[ind_size_inv][l];
                    directions_result[index_total] = retrieved_directions_inv[ind_size_inv][l];
                    index_total++;
                }
            }
            if(debug)
            {
                std::cout << std::endl;
                std::cout << "Number of points processed on realign and previous order is: " << index_total << std::endl;
            }

            (*mFiberClusterVector)[i].setNumVerticesPerFiber(num_vert_per_fiber);
            (*mFiberClusterVector)[i].setNumFiberSegmentPoints(processed_vertices);

            GLuint vertices_final_VBO;
            glGenBuffers(1, &vertices_final_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, vertices_final_VBO);
            {
                glBufferData(GL_ARRAY_BUFFER, processed_vertices * sizeof(GLfloat) * 4, vertices_result, GL_STATIC_DRAW);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);


            GLuint directions_final_VBO;
            glGenBuffers(1, &directions_final_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, directions_final_VBO);
            {
                glBufferData(GL_ARRAY_BUFFER, processed_vertices * sizeof(GLfloat) * 4, directions_result, GL_STATIC_DRAW);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);


            //Set Fiber Directions and Vertices on ClusterVector Final Result
            (*mFiberClusterVector)[i].setVBOFiberDirections(directions_final_VBO);
            (*mFiberClusterVector)[i].setVBOFiberVertices(vertices_final_VBO);


            glDeleteBuffers(1, &seedsVBO);
            glDeleteBuffers(1, &verticesVBO);

            delete[] fiber_size_orig;
            delete[] fiber_size_inv;
            delete[] fiber_size_orig_id;
            delete[] fiber_size_inv_id;
            delete[] seeds_loops_orig;
            delete[] vertices_result;
            delete[] directions_result;

        }
    }

    mProgramGSFiberTracking->releasing();
}

unsigned int FiberTrackingAlgorithmGeometryShader::trackFibers(unsigned int VBO_id_fibers, unsigned int VBO_id_seeds, unsigned int num_seeds, FiberCluster::MyVec4 * feedback, bool debug)
{
    GLuint query;
    GLuint primitives;
    glGenQueries(1, &query);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO_id_fibers);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO_id_seeds);
        {
            glEnableVertexAttribArray(mAttribLocSeedPosition);
            glVertexAttribPointer(mAttribLocSeedPosition, 4, GL_FLOAT, false, 8 * sizeof(GLfloat), BUFFER_OFFSET(0));

            if(debug)
            {
                std::cout << "glAttribPointer mSeedsVBO " << glGetError() << std::endl;
            }
            glEnableVertexAttribArray(mAttribLocDirection);
            glVertexAttribPointer(mAttribLocDirection, 4, GL_FLOAT, false, 8 * sizeof(GLfloat), BUFFER_OFFSET(sizeof(GLfloat)*4));

            if(debug)
            {
                std::cout << "glAttribPointer mDirVBO " << glGetError() << std::endl;
            }

            glEnable(GL_RASTERIZER_DISCARD);
            {
                glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
                {
                    if(debug)
                    {
                        std::cout << "glBeginTransformFeedback mTractsVBO " << glGetError() << std::endl;
                    }
                    glBeginTransformFeedback(GL_POINTS);
                    glDrawArrays(GL_POINTS, 0, num_seeds);
                    glEndTransformFeedback();
                }
                glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
            }
            glDisable(GL_RASTERIZER_DISCARD);

            glFlush();

            //Get Results
            glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

            glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(float)*4*primitives, feedback);

            if(debug)
            {
                std::cout << "glEndTransformFeedback mTractsVBO " << glGetError() << std::endl;
            }

            glDisableVertexAttribArray(mAttribLocDirection);
            glDisableVertexAttribArray(mAttribLocSeedPosition);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glDeleteQueries(1, &query);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

    return primitives;
}

void FiberTrackingAlgorithmGeometryShader::setFiberStepGeometryShader()
{
    float * voxelDim = getVoxelDim();

    if(voxelDim[0] < voxelDim[1])
    {
        if(voxelDim[0] < voxelDim[2])
            m_fiber_step = voxelDim[0];
        else
            m_fiber_step = voxelDim[2];

    } else if(voxelDim[1] < voxelDim[2])
        m_fiber_step = voxelDim[1];
    else
        m_fiber_step = voxelDim[2];

    m_fiber_step = m_fiber_step / 4.f;

    setFiberStep(m_fiber_step);

    mProgramGSFiberTracking->binding();
    glUniform1f(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "fiberStep"), m_fiber_step);
}

void FiberTrackingAlgorithmGeometryShader::setStoppingCriterionFA(float stopCriterionFA)
{
    FiberTrackingAlgorithm::setStoppingCriterionFA(stopCriterionFA);

    mProgramGSFiberTracking->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "stoppingCriterionFA"), getFiberTrackingStoppingCriterionFA());
    }
    mProgramGSFiberTracking->releasing();
}

void FiberTrackingAlgorithmGeometryShader::setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle)
{
    FiberTrackingAlgorithm::setStoppingCriterionAngle(fiberTrackingStoppingCriterionAngle);

    mProgramGSFiberTracking->binding();
    {
        std::cout << "GS setStoppingCriterionAngle " << getFiberTrackingStoppingCriterionAngleCos() << " err " << glGetError() << std::endl;
        glUniform1f(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "stoppingCriterionAngleCos"), getFiberTrackingStoppingCriterionAngleCos());
    }
    mProgramGSFiberTracking->releasing();
}

void FiberTrackingAlgorithmGeometryShader::setWpunct(float value)
{
    setFiberTrackingWpunct(value);

    mProgramGSFiberTracking->binding();
    {
        std::cout << "w_punct " << getFiberTrackingWpunct() << " err " << glGetError() << std::endl;
        glUniform1f(glGetUniformLocation(mProgramGSFiberTracking->getProgram(), "w_punct"), getFiberTrackingWpunct());
    }
    mProgramGSFiberTracking->releasing();
}
