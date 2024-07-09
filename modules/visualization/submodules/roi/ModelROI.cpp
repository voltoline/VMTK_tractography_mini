#include "ModelROI.h"

#include <modules/visualization/submodules/2d_transformation/model/Model2DTransformation.h>

#include <modules/visualization/submodules/cursor/event_handler/EventHandler2DTransformationToCursor.h>

#include <fstream>

static const float EPSILON=0.0000000001f;

#define MAX(a,b) ((a) > (b) ? (a) : (b))  /**< maximum among the input values */
#define MIN(a,b) ((a) < (b) ? (a) : (b))  /**< minimum among the input values */

ModelROI::ModelROI()
{
    m_enable_render = false;
    m_enable_free_render = false;
    m_last_removed_roi_index = -1;
    m_modelCursor = new ModelCursor();
    m_modelCursor->setFlagSync2DSlice(false);
    id_generator = 0;
    m_controller_stroke_draw_activation = false;
    m_roiStateChangedHandlerSlotID = -1;
    m_isEnabled = false;
    m_stateManager = nullptr;
    m_model_changed = false;

    m_roiEditionActivated = false;

    m_eventHandlerCursor = new EventHandler2DTransformationToCursor();
    m_eventHandlerCursor->setModelCursor(m_modelCursor);

    m_currentEditableTractID = -1;

    m_model2DSlice = nullptr;
    m_model2DTransformation = nullptr;
    m_mainWindow = nullptr;
}

ModelROI::~ModelROI()
{
    if(m_stateManager != nullptr)
        m_stateManager->unsubscribe("ROI", m_roiStateChangedHandlerSlotID);
}

void ModelROI::setDataAcquisition(unsigned short dim_x, unsigned short dim_y, unsigned short dim_z,
                                  double pixelSpacing_x, double pixelSpacing_y, double pixelSpacing_z, std::string id)
{
    m_patient_id = id;

    m_dimension[0] = static_cast<int>(dim_x);
    m_dimension[1] = static_cast<int>(dim_y);
    m_dimension[2] = static_cast<int>(dim_z);

    m_pixelSpacing[0] = pixelSpacing_x;
    m_pixelSpacing[1] = pixelSpacing_y;
    m_pixelSpacing[2] = pixelSpacing_z;

    for(int i = 0; i < 3; i++)
    {
        m_real_dimension[i] = m_dimension[i] * m_pixelSpacing[i];
    }

    m_real_maxDimension = MAX(MAX(m_real_dimension[0], m_real_dimension[1]), m_real_dimension[2]);

    for (int i = 0; i < 3; i++)
    {
        m_fScaleFactors[i] = m_real_dimension[i] / m_real_maxDimension;
    }

    m_fScaleFactors[3] = 1.0;

    m_maxDimension = MAX(MAX(m_dimension[0], m_dimension[1]), m_dimension[2]);

    float phyScale[3];

    phyScale[0] = static_cast<float>((m_pixelSpacing[0]) / m_real_maxDimension);
    phyScale[1] = static_cast<float>((m_pixelSpacing[1]) / m_real_maxDimension);
    phyScale[2] = static_cast<float>((m_pixelSpacing[2]) / m_real_maxDimension);

    m_modelCursor->setPhyScale(phyScale);
    m_modelCursor->setScaleFactors(m_fScaleFactors);
}

void ModelROI::setMainWindow(MainWindow * main){
    m_mainWindow = main;
}

MainWindow * ModelROI::getMainWindow()
{
    return m_mainWindow;
}

ModelROISetEvent ModelROI::newROI(roi_type type, std::string tract_name, int tract_id)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::newROI -->> A notification is in progress!" << std::endl;
    }

    roi * newROI = new roi();
    newROI->id = new_id();
    newROI->type = type;
    newROI->plane = ModelROI::NON_INITIALAZED;
    newROI->tract_name = tract_name;
    newROI->tract_id = tract_id;

    //Add to ROI List with
    m_roi.push_back(*newROI);

    //Set Current Active ROI as the just created
    setCurrentActiveROIid(newROI->id);

    ModelROISetEvent event = setEnableRendering(true);
    notify(event, nullptr);

    return ModelROISetEvent::ROI_CREATED;
}

ModelROISetEvent ModelROI::addROI(roi new_roi)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::newROI -->> A notification is in progress!" << std::endl;
    }

    //Add to ROI List with
    m_roi.push_back(new_roi);

    //Set Current Active ROI as the just created
    setCurrentActiveROIid(new_roi.id);

    if(new_roi.id >= id_generator)
        id_generator = new_roi.id + 1;

    ModelROISetEvent event = setEnableRendering(true);
    notify(event, nullptr);

    return ModelROISetEvent::ROI_ADDED;
}

ModelROISetEvent ModelROI::newROI(roi_type type, roi_plane plane, int id)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::newROI -->> A notification is in progress!" << std::endl;
    }

    roi * newROI = new roi();
    newROI->id = id;
    newROI->type = type;
    newROI->plane = plane;
    newROI->tract_id = m_currentEditableTractID;

    //Add to ROI List with
    m_roi.push_back(*newROI);

    //Set Current Active ROI as the just created
    setCurrentActiveROIid(newROI->id);

    ModelROISetEvent event = setEnableRendering(true);
    notify(event, nullptr);

    return ModelROISetEvent::ROI_CREATED;
}

ModelROISetEvent ModelROI::setROIEditionActivated(bool state)
{
    m_roiEditionActivated = state;

    return ModelROISetEvent::ROI_EDITION;
}

void ModelROI::setNameROI(unsigned int id, std::string name)
{
    if(m_roi.size() > 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "Can't Find ROI index for Removal" << std::endl;
                return;
            }
        }
        m_roi[index_id].name = name;

    }
}

void ModelROI::setTractROI(unsigned int id, std::string name_tract, int id_tract)
{
    if(m_roi.size() > 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "Can't Find ROI index for Tract Settings" << std::endl;
                return;
            }
        }

        m_roi[index_id].tract_name = name_tract;
        m_roi[index_id].tract_id = id_tract;

    }
}

std::vector<int> ModelROI::getROIidsFromTractId(int tract_id)
{
    std::vector<int> roi_ids_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].tract_id == tract_id)
            roi_ids_information.push_back(m_roi[i].id);
    }

    return roi_ids_information;
}

int ModelROI::getTractID(unsigned int roi_id)
{
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(roi_id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "ModelROI::getTractID ->> Can't Find ROI index" << std::endl;
                return -1;
            }
        }
    }

    std::cout << "ModelROI::getTractID " << m_roi[index_id].tract_id << std::endl;

    return m_roi[index_id].tract_id;
}

ModelROISetEvent ModelROI::removeROI(unsigned int id, bool tract_removed)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::removeROI -->> A notification is in progress!" << std::endl;
    }

    if(m_roi.size() > 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "Can't Find ROI index for Removal" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
        //************ CHECK CONSISTENCY AFTER SEVERAL REMOVALS AN INSERTIONS **************
        m_roi.erase(m_roi.begin() + index_id);

        //Set Index from removed ROI
        m_last_removed_roi_index = index_id;
        m_last_removed_roi_id = id;

        if(m_roi.size() == 0)
        {
            std::cout << "Removed Last Element, stop rendering Stroke ROI" << std::endl;
            //No ROI's on List, disable RENDERING
            m_current_active_ROI_index = -1;
            ModelROISetEvent event = setEnableRendering(false);
            notify(event, nullptr);

            if(!tract_removed)
            {
                ModelROISetEvent event_change = setModelChange(true);
                notify(event_change, nullptr);
            }

            return ModelROISetEvent::LAST_ROI_REMOVED;
        }
        else
        {
            //Set Active ROI as last Element of list
            setCurrentActiveROIindex(m_roi.size() - 1);
        }

        if(!tract_removed)
        {
            ModelROISetEvent event_change = setModelChange(true);
            notify(event_change, nullptr);
        }

        return ModelROISetEvent::ROI_REMOVED;

    }
    else
    {
        std::cerr << "Attempt to remove ROI on empty list" << std::endl;
    }

    return ModelROISetEvent::INDEX_ERROR;
}

ModelROISetEvent ModelROI::addPoint(unsigned int id, glm::vec3 point)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::addPoint -->> A notification is in progress!" << std::endl;
    }

    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "Can't Find ROI index for Point Addition" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
    }
    m_roi[index_id].points.push_back(point);

    //Check if point was integrated
    m_last_point_added = point;
    return ModelROISetEvent::POINT_ADDED;
}

glm::vec3 ModelROI::getLastPointAdded()
{
    return m_last_point_added;
}

ModelROISetEvent ModelROI::endROI(unsigned int id)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::endROI -->> A notification is in progress!" << std::endl;
    }

    //Add First element as last point to make a circular list of points
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
       std::cout << "Size of ROI Being closed: " << m_roi[index_id].points.size() << std::endl;
        /* Circular linking: the last with the first */
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "Can't Find ROI index for Closure" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
        if (m_roi[index_id].points.size() > 2)
        {
            m_roi[index_id].points.push_back(m_roi[index_id].points[0]);
            ModelROISetEvent event_change = setModelChange(true);

            //Ting: update slce
            notify(event_change, nullptr);
        }
        else
        {
            /* If no point was added to the list, remove the ROI. */
            ModelROISetEvent event = removeROI(m_roi[index_id].id, false);
            return event;
        }
    }

    m_roiEditionActivated = false;
    return ModelROISetEvent::ROI_ENDED;
}

ModelROISetEvent ModelROI::setType(unsigned int id, roi_type type)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setType -->> A notification is in progress!" << std::endl;
    }

    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Type set" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
    }

    roi & desiredROI = m_roi.at(index_id);
    desiredROI.type = type;
    return ModelROISetEvent::CHANGE_TYPE_ROI;
}

void ModelROI::setModel2DSlice(Model2DSlice *model2DSlice)
{
    m_model2DSlice = model2DSlice;
    m_modelCursor->setModel2DSlice(m_model2DSlice);
}

ModelCursor *ModelROI::getModelCursor() const
{
    return m_modelCursor;
}

glm::vec3 ModelROI::getMousePosition3D()
{
    return m_modelCursor->getCursorPosition3D();
}

glm::vec3 ModelROI::getMousePositionAxial()
{
    return m_modelCursor->getCursorPositionAxial();
}

glm::vec3 ModelROI::getMousePositionSagittal()
{
    return m_modelCursor->getCursorPositionSagittal();
}

std::vector<int> ModelROI::getIndexes_seed_concatenation() const
{
    return m_indexes_seed_concatenation;
}

std::vector<int> ModelROI::getIndexes_not_concatenation() const
{
    return m_indexes_not_concatenation;
}

std::vector<int> ModelROI::getIndexes_and_concatenation() const
{
    return m_indexes_and_concatenation;
}

std::vector<int> ModelROI::getNumberFilterAND() const
{
        return m_number_fiter_and_concatenation_index;
}

std::vector<int> ModelROI::getNumberFilterNOT() const
{
    return m_number_fiter_not_concatenation_index;
}

void ModelROI::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;

    m_modelCursor->setModelViewMatrixAxial(m_model2DTransformation->getAxialData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixAxial(m_model2DTransformation->getAxialData().getProjectionMatrix());

    m_modelCursor->setModelViewMatrixCoronal(m_model2DTransformation->getCoronalData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixCoronal(m_model2DTransformation->getCoronalData().getProjectionMatrix());

    m_modelCursor->setModelViewMatrixSagittal(m_model2DTransformation->getSagittalData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixSagittal(m_model2DTransformation->getSagittalData().getProjectionMatrix());

    m_eventHandlerCursor->setModel2DTransformation(m_model2DTransformation);
}

glm::vec3 ModelROI::getMousePositionCoronal()
{
    return m_modelCursor->getCursorPositionCoronal();
}

ModelROI::roi_type ModelROI::getType(unsigned int id)
{
    bool error = false;
    if(m_roi.size() != 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Plane set" << std::endl;
                error = true;
                break;
            }
        }

        if(!error)
            return m_roi.at(index_id).type;
    }

    std::cerr << "Couldn't find ROI with id " << id << " for Plane set" << std::endl;
}

ModelROISetEvent ModelROI::setPlane(unsigned int id, roi_plane plane)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setPlane -->> A notification is in progress!" << std::endl;
    }

    //Change ROI Plane
    if(m_roi.size() != 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Plane set" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
        roi & desiredROI = m_roi.at(index_id);
        desiredROI.plane = plane;

        if (desiredROI.plane == ModelROI::AXIAL_2D)
        {
            desiredROI.slice = m_model2DSlice->getAxialSlice();
        }
        else if (desiredROI.plane == ModelROI::SAGITTAL_2D)
        {
            desiredROI.slice = m_model2DSlice->getSagittalSlice();
        }
        else if(desiredROI.plane == ModelROI::CORONAL_2D)
        {
            desiredROI.slice = m_model2DSlice->getCoronalSlice();
        }

    }
    return ModelROISetEvent::ROI_PLANE_CHANGE;
}

ModelROI::roi_plane ModelROI::getPlane(unsigned int id)
{
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Plane retrieval" << std::endl;
                break;
            }
        }
    }
    roi & desiredROI = m_roi.at(index_id);
    return desiredROI.plane;
}

std::vector<glm::vec3> ModelROI::getROIpoints(unsigned int id)
{
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Points retrieval" << std::endl;
                break;
            }
        }
    }
    return m_roi[index_id].points;
}

std::vector<int> ModelROI::bresenham3DAlgorithm(int x0, int y0, int z0, int x1, int y1, int z1, bool debug)
{
    int dx, dy, dz;
    int x_inc, y_inc, z_inc;
    int l, m, n, i;
    int err_1, err_2;
    int dx2, dy2, dz2;
    int point[3];
    point[0] = x0;
    point[1] = y0;
    point[2] = z0;
    if (debug)
        std::cout << "Original Point Bresenham: " << point[0] << " " << point[1] << " " << point[2] << " " << std::endl;

    std::vector<int> result;

    dx = x1 - x0;
    dy = y1 - y0;
    dz = z1 - z0;

    x_inc = (dx < 0) ? -1 : 1;
    y_inc = (dy < 0) ? -1 : 1;
    z_inc = (dz < 0) ? -1 : 1;

    l = abs(dx);
    m = abs(dy);
    n = abs(dz);

    dx2 = l << 1; //2*l; //l << 1; //l * 2^1
    dy2 = m << 1; //2*m; //<< 1;
    dz2 = n << 1; //2*n; // << 1;

    if ((l >= m) && (l >= n)) {
            err_1 = dy2 - l;
            err_2 = dz2 - l;
            for (i = 0; i < l; i++) {
                if(debug)
                    std::cout << "New Point Bresenham: " << point[0] << " " << point[1] << " " << point[2] << " " << std::endl;
                result.push_back(point[0]);
                result.push_back(point[1]);
                result.push_back(point[2]);
                if (err_1 > 0) {
                    point[1] += y_inc;
                    err_1 -= dx2;
                }
                if (err_2 > 0) {
                    point[2] += z_inc;
                    err_2 -= dx2;
                }
                err_1 += dy2;
                err_2 += dz2;
                point[0] += x_inc;
            }
        } else if ((m >= l) && (m >= n)) {
            err_1 = dx2 - m;
            err_2 = dz2 - m;
            for (i = 0; i < m; i++) {
                if(debug)
                    std::cout << "New Point Bresenham: " << point[0] << " " << point[1] << " " << point[2] << " " << std::endl;
                result.push_back(point[0]);
                result.push_back(point[1]);
                result.push_back(point[2]);
                if (err_1 > 0) {
                    point[0] += x_inc;
                    err_1 -= dy2;
                }
                if (err_2 > 0) {
                    point[2] += z_inc;
                    err_2 -= dy2;
                }
                err_1 += dx2;
                err_2 += dz2;
                point[1] += y_inc;
            }
        } else {
            err_1 = dy2 - n;
            err_2 = dx2 - n;
            for (i = 0; i < n; i++) {
                if(debug)
                    std::cout << "New Point Bresenham: " << point[0] << " " << point[1] << " " << point[2] << " " << std::endl;
                result.push_back(point[0]);
                result.push_back(point[1]);
                result.push_back(point[2]);
                if (err_1 > 0) {
                    point[1] += y_inc;
                    err_1 -= dz2;
                }
                if (err_2 > 0) {
                    point[0] += x_inc;
                    err_2 -= dz2;
                }
                err_1 += dy2;
                err_2 += dx2;
                point[2] += z_inc;
            }
        }
    if(debug)
        std::cout << "Last Point Bresenham: " << point[0] << " " << point[1] << " " << point[2] << " " << std::endl;

    return result;
}

float ModelROI::area(std::vector<float> contour)
{

  int n = contour.size();

  float A=0.0f;

  for(int p=n-2,q=0; q<n; q+=2,p=q)
  {
    A+= contour[p]*contour[q+1] - contour[q]*contour[p+1];
  }
  return A*0.5f;
}

/*
  insideTriangle decides if a point P is Inside of the triangle defined by A, B, C.
*/
bool ModelROI::insideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy,float Px, float Py)
{
float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
float cCROSSap, bCROSScp, aCROSSbp;

ax = Cx - Bx;  ay = Cy - By;
bx = Ax - Cx;  by = Ay - Cy;
cx = Bx - Ax;  cy = By - Ay;
apx= Px - Ax;  apy= Py - Ay;
bpx= Px - Bx;  bpy= Py - By;
cpx= Px - Cx;  cpy= Py - Cy;

aCROSSbp = ax*bpy - ay*bpx;
cCROSSap = cx*apy - cy*apx;
bCROSScp = bx*cpy - by*cpx;

return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
}

bool ModelROI::snip(std::vector<float> contour,int u,int v,int w,int n,int *V)
{
  int p;
  float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

  Ax = contour[V[u]];
  Ay = contour[V[u]+1];

  Bx = contour[V[v]];
  By = contour[V[v]+1];

  Cx = contour[V[w]];
  Cy = contour[V[w]+1];

  if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) ) return false;

  for (p=0;p<n;p++)
  {
    if( (p == u) || (p == v) || (p == w) ) continue;
    Px = contour[V[p]];
    Py = contour[V[p]+1];
    if (insideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) return false;
  }

  return true;
}

bool ModelROI::triangulatePolygon(std::vector<float> contour, std::vector<float> result)
{
  /* allocate and initialize list of Vertices in polygon */

  int n = contour.size()/2;
  if ( n < 3 ) return false;

  int *V = new int[n];

  /* we want a counter-clockwise polygon in V */

  if ( 0.0f < area(contour) )
    for (int v=0; v<n; v++) V[v] = v;
  else
    for(int v=0; v<n; v++) V[v] = (n-1)-v;

  int nv = n;

  /*  remove nv-2 Vertices, creating 1 triangle every time */
  int count = 2*nv;   /* error detection */

  for(int m=0, v=nv-1; nv>2; )
  {
    /* if we loop, it is probably a non-simple polygon */
    if (0 >= (count--))
    {
      //** Triangulate: ERROR - probable bad polygon!
      return false;
    }

    /* three consecutive vertices in current polygon, <u,v,w> */
    int u = v  ; if (nv <= u) u = 0;     /* previous */
    v = u+1; if (nv <= v) v = 0;     /* new v    */
    int w = v+1; if (nv <= w) w = 0;     /* next     */

    if ( snip(contour,u,v,w,nv,V) )
    {
      int a,b,c,s,t;

      /* true names of the vertices */
      a = V[u]; b = V[v]; c = V[w];

      /* output Triangle */
      result.push_back( contour[a] );
      result.push_back( contour[b] );
      result.push_back( contour[c] );

      m++;

      /* remove v from remaining polygon */
      for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;

      /* resest error detection counter */
      count = 2*nv;
    }
  }

  delete[] V;

  return true;
}

void ModelROI::setCurrentEditableTractID(int currentEditableTractID)
{
    m_currentEditableTractID = currentEditableTractID;
}

bool ModelROI::getRoiEditionActivated() const
{
    return m_roiEditionActivated;
}


void ModelROI::computeCentroid2D(std::vector<int> pos, double * centr)
{
    //Reference https://en.wikipedia.org/wiki/Centroid#Of_a_polygon

    double signedArea = 0;
    int p0[2], p1[2];
    double a;
    unsigned int i = 01;

    for(i = 0; i < pos.size()-4; i = i + 2)
    {
        p0[0] = pos[i];        //x
        p0[1] = pos[i+1];    //y

        p1[0] = pos[i+2];         //x_p
        p1[1] = pos[i+3];       //y_p

        a = p0[0]*p1[1] - p1[0]*p0[1]; //x*y_p - x_p*y
        signedArea += a;

        centr[0] += (p0[0] + p1[0])*a;
        centr[1] += (p0[1] + p1[1])*a;
    }

    //Last Point
    p0[0] = pos[i];        //x
    p0[1] = pos[i+1];    //y

    p1[0] = pos[0];         //x_p
    p1[1] = pos[1];       //y_p

    a = p0[0]*p1[1] - p1[0]*p0[1]; //x*y_p - x_p*y
    signedArea += a;

    centr[0] += (p0[0] + p1[0])*a;
    centr[1] += (p0[1] + p1[1])*a;

    signedArea*= 0.5;
    centr[0] = centr[0]/(6.0*signedArea);
    centr[1] = centr[1]/(6.0*signedArea);

    //Check whether (centr[0],centr[1]) is inside the polygon   (Ting, 29/04/2022)
    //(https://www.codeproject.com/Tips/84226/Is-a-Point-inside-a-Polygon)
    unsigned int j, k = 0, c=0;
    int tmp, x[20];
    for(i = 0, j = pos.size()-2; i <= pos.size()-2; j=i, i = i + 2)
    {
        if ((pos[i+1]>centr[1]) != (pos[j+1]>centr[1])) {
             tmp = ((pos[j]-pos[i]) * (centr[1]-pos[i+1]) / (pos[j+1]-pos[i+1])) + pos[i];
             if (k<20) x[k++] = tmp;
             if (centr[0] < tmp)  c = !c;
        }
    }

    //If the computed centroid is out, take an interior point (Ting, 29/04/2022)
    if (c == 0) {

      centr[0] = (x[0]+x[1])/2;
      for(c = 0, i = 0, j = pos.size()-2; i <= pos.size()-2; j=i, i = i + 2)
      {
          if ((pos[i+1]>centr[1]) != (pos[j+1]>centr[1])) {
               tmp = ((pos[j]-pos[i]) * (centr[1]-pos[i+1]) / (pos[j+1]-pos[i+1])) + pos[i];
               if (centr[0] < tmp)  c = !c;
          }
      }
      if (c == 0 && k > 2) {
          centr[0] = (x[1]+x[2])/2;
      }
    }

    return;
}

void ModelROI::get2DPointsOf3DPlane(std::vector<int> in_3D, std::vector<int> &out_2D, int *plane)
{
    bool x_plane= true, y_plane = true, z_plane = true;

    //Find Plane(variable) which does not alter
    for(unsigned int i = 0; i <= in_3D.size()-6; i = i + 3)
    {
        if(in_3D[i] != in_3D[i+3])
            x_plane = false;
        if(in_3D[i+1] != in_3D[i+4])
            y_plane = false;
        if(in_3D[i+2] != in_3D[i+5])
            z_plane = false;
    }

    //Check if region is  not aligned
    if((x_plane && y_plane) || (x_plane && z_plane) || (y_plane && z_plane))
    {
        *plane = 0;
        return;
    }

    if(x_plane)
    {
        *plane = 1;
        for(unsigned int i = 0; i < in_3D.size(); i = i + 3)
        {
            out_2D.push_back(in_3D[i+1]);
            out_2D.push_back(in_3D[i+2]);
        }
    }
    else if(y_plane)
    {
        *plane = 2;
        for(unsigned int i = 0; i < in_3D.size(); i = i + 3)
        {
            out_2D.push_back(in_3D[i]);
            out_2D.push_back(in_3D[i+2]);
        }
    }
    else if(z_plane)
    {
        *plane = 3;
        for(unsigned int i = 0; i < in_3D.size(); i = i + 3)
        {
            out_2D.push_back(in_3D[i]);
            out_2D.push_back(in_3D[i+1]);
        }
    }

}

void ModelROI::computeCentroid3D(std::vector<int> p, int * centroid)
{
    std::vector<int> p_2D;
    int plane;  //x - 1, y - 2, z = 3

    get2DPointsOf3DPlane(p, p_2D, &plane);

    if(plane == 0)
    {
        //Voxels are aligned, return centroid as zero
        centroid[0] = 0;
        centroid[1] = 0;
        centroid[2] = 0;
        return;

    }

    //Call the centroid 2D with new set of points
    double local_centroid[2] = {0.0, 0.0};
    computeCentroid2D(p_2D, local_centroid);

    if(plane == 1) //x
    {
        centroid[0] = p[0];
        centroid[1] = static_cast<int>(round(local_centroid[0]));
        centroid[2] = static_cast<int>(round(local_centroid[1]));
    }
    else if(plane == 2) //y
    {
        centroid[0] = static_cast<int>(round(local_centroid[0]));
        centroid[1] = p[1];
        centroid[2] = static_cast<int>(round(local_centroid[1]));
    }
    else if (plane == 3) //z
    {
        centroid[0] = static_cast<int>(round(local_centroid[0]));
        centroid[1] = static_cast<int>(round(local_centroid[1]));
        centroid[2] = p[2];
    }

}

int ModelROI::findInteriorVoxels(std::vector<int> p, int x, int y, int z, std::vector<int> &indexes, bool debug)
{
    /**Parameter: indexes is a return List with 2D coordinates*/
    /**Parameter: p is a the 3D boundary Points*/
    /**Parameter: x, y, z is a point inside Boundary*/
    /**Return Value: Indicate the plane used on the 2D Coordinates*/

    std::vector<int> p_2D;
    int plane;  //x - 1, y - 2, z = 3
    int size_1, size_2;
    int p1, p2;

    get2DPointsOf3DPlane(p, p_2D, &plane);

//    if(debug)
//        std::cout << "ModelROI::findInteriorVoxels:Plane of Choice: " << plane << std::endl;

    if(plane == 0)
    {
        //Voxels are aligned, so return interior voxels only as boundary points
        for (unsigned int i = 0; i < p.size(); i++)
        {
            indexes.push_back(p[i]);
        }
        return plane;
    }

    if(plane == 1)
    {
        size_1 = m_dimension[1];
        size_2 = m_dimension[2];
        p1 = y;
        p2 = z;
    }
    else if (plane == 2)
    {
        size_1 = m_dimension[0];
        size_2 = m_dimension[2];
        p1 = x;
        p2 = z;
    }
    else
    {
        size_1 = m_dimension[0];
        size_2 = m_dimension[1];
        p1 = x;
        p2 = y;
    }

    std::vector<std::vector<int>> search_matrix( size_1, std::vector<int>(size_2));

    for(std::vector<int>::iterator it = p_2D.begin(); it != p_2D.end(); std::advance(it, 2))
    {
        search_matrix[*it][*(it+1)] = 1;    //Create Color Matrix
    }

    floodFill(search_matrix, p1, p2, size_1, size_2);

    //Print Search Matrix
    for(int i = 0; i < size_1; i++)
    {
        for(int j = 0; j < size_2; j++)
        {
            if(search_matrix[i][j] == 1)
            {
                indexes.push_back(i);
                indexes.push_back(j);
            }

        }
    }

    return plane;
}

void ModelROI::floodFill(std::vector<std::vector<int>> &color, int x, int y, int max_x, int max_y)
{
    if (x >= max_x || y >= max_y || x < 0 || y < 0)
    {
        return;
    }

    if(color[x][y] == 1)
        return;
    else
    {
        color[x][y] = 1;
        floodFill(color, x+1, y, max_x, max_y);
        floodFill(color, x, y+1, max_x, max_y);
        floodFill(color, x-1, y, max_x, max_y);
        floodFill(color, x, y-1, max_x, max_y);
    }
}

void ModelROI::computeIndexes(unsigned int id)
{
    std::vector<int> ROI_indexes;
    std::vector<int> ROI_indexes_closed;

    //Compute all points from ROI that defined the lines between mark points
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Indexes computation" << std::endl;
                return;
            }
        }
    }

    //Add Condition to check if indexes are already computed
    if(m_roi.at(index_id).indexes.size() > 0)
    {
        return /*ModelROISetEvent::ROI_INDEXES_COMPUTED*/;
    }

    roi & desiredROI = m_roi.at(index_id);
    std::vector<glm::vec3> markPoints = desiredROI.points;
    std::vector<glm::vec3> markPointsPhysical;

    for (unsigned int k = 0; k < markPoints.size(); k++)
    {
        glm::vec3 pos3D;
        //Normalize from [1,1] to [0,1], norm = (x-min)/(max-min)
        pos3D.x = (markPoints[k].x  + m_fScaleFactors[0])/2;
        pos3D.y = (markPoints[k].y  + m_fScaleFactors[1])/2;
        pos3D.z = (markPoints[k].z  + m_fScaleFactors[2])/2;

        if (pos3D.x < 0 || pos3D.x > m_fScaleFactors[0] || pos3D.y < 0 || pos3D.y > m_fScaleFactors[1] || pos3D.z < 0 || pos3D.z > m_fScaleFactors[2]) {
            //Cursor located at invalid position, i. e., outside image volume space
            std::cerr << "Cursor outside Volume Space, voxels not computed" << std::endl;
        } else {


        glm::vec3 point3D;    //Point Dimension
        point3D.x = pos3D.x * m_real_maxDimension;  //width
        point3D.y = pos3D.y * m_real_maxDimension;  //height
        point3D.z = pos3D.z * m_real_maxDimension;  //depth

        int voxelPoint3D[3];

        voxelPoint3D[0] = static_cast<int> (point3D.x / m_pixelSpacing[0]);
        voxelPoint3D[1] = static_cast<int> (point3D.y / m_pixelSpacing[1]);
        voxelPoint3D[2] = static_cast<int> (point3D.z / m_pixelSpacing[2]);

        if(m_roi.at(index_id).plane == roi_plane::AXIAL_2D)
        {
            voxelPoint3D[2] = voxelPoint3D[2] - 1;
        }
        else if (m_roi.at(index_id).plane == roi_plane::SAGITTAL_2D)
        {
            voxelPoint3D[0] = voxelPoint3D[0] - 1;
        }

       //Add Point and Indexes to list
        markPointsPhysical.push_back(point3D);
        ROI_indexes.push_back(voxelPoint3D[0]);
        ROI_indexes.push_back(voxelPoint3D[1]);
        ROI_indexes.push_back(voxelPoint3D[2]);
        }

    }

    /*************************** Test Points ********************************************************/

    unsigned int size = ROI_indexes.size();
    if(size >= 6)
    {
        for(unsigned int i = 0; i <= size-6; i = i + 3)
        {
            int p_x = ROI_indexes[i];
            int p_y = ROI_indexes[i+1];
            int p_z = ROI_indexes[i+2];

            int p_1_x = ROI_indexes[i+3];
            int p_1_y = ROI_indexes[i+4];
            int p_1_z = ROI_indexes[i+5];

            //BEFORE CALL BRESENHAM CHECK IF IT IS NOT A CLOSED SET OR A SINGLE VOXEL
            std::vector<int> parcialResult;
            if(p_x == p_1_x && p_y == p_1_y && p_z == p_1_z)
            {
                parcialResult.push_back(p_x);
                parcialResult.push_back(p_y);
                parcialResult.push_back(p_z);
            }
            else
            {
                parcialResult = bresenham3DAlgorithm(p_x, p_y, p_z, p_1_x, p_1_y, p_1_z, false);
            }

            for(unsigned int j = 0; j < parcialResult.size(); j = j + 3)
            {
                int pr_x = parcialResult[j];
                int pr_y = parcialResult[j+1];
                int pr_z = parcialResult[j+2];

                //Add all points to final list
                ROI_indexes_closed.push_back(pr_x);
                ROI_indexes_closed.push_back(pr_y);
                ROI_indexes_closed.push_back(pr_z);
            }
        }
        //Compute Voxel Inside Defined Region
        int centroid[3] = {0, 0, 0};
        computeCentroid3D(ROI_indexes_closed, centroid);

        //Compute Internal Voxels
        std::vector<int> internal_voxel_indexes;
        int plane = findInteriorVoxels(ROI_indexes_closed, centroid[0], centroid[1], centroid[2], internal_voxel_indexes, true);
        int ref;
        if(plane == 1)
        {
            ref = ROI_indexes_closed[0];
        }
        else if (plane == 2)
        {
            ref = ROI_indexes_closed[1];
        }
        else
        {
            ref = ROI_indexes_closed[2];
        }

        std::vector<int> final_voxels;
        if(plane != 0)
        {
            for(unsigned int j = 0; j < internal_voxel_indexes.size(); j = j + 2)
            {
                if(plane == 1)
                {
                    final_voxels.push_back(ref);
                    final_voxels.push_back(internal_voxel_indexes[j]);
                    final_voxels.push_back(internal_voxel_indexes[j+1]);
                }
                else if( plane == 2)
                {
                    final_voxels.push_back(internal_voxel_indexes[j]);
                    final_voxels.push_back(ref);
                    final_voxels.push_back(internal_voxel_indexes[j+1]);
                }
                else
                {
                    final_voxels.push_back(internal_voxel_indexes[j]);
                    final_voxels.push_back(internal_voxel_indexes[j+1]);
                    final_voxels.push_back(ref);
                }
            }
        }
        else
        {
            for(unsigned int j = 0; j < internal_voxel_indexes.size(); j = j + 3)
            {
                bool already_added = false;
                for (unsigned int k = 0; k < final_voxels.size(); k = k + 3)
                {
                    if(internal_voxel_indexes[j] == final_voxels[k] && internal_voxel_indexes[j+1] == final_voxels[k+1] && internal_voxel_indexes[j+2] == final_voxels[k+2])
                    {
                        already_added = true;
                        break;
                    }
                }
                if(!already_added)
                {
                    final_voxels.push_back(internal_voxel_indexes[j]);
                    final_voxels.push_back(internal_voxel_indexes[j+1]);
                    final_voxels.push_back(internal_voxel_indexes[j+2]);
                }
            }
        }

        //Save ROI Index allong the Line
        m_roi.at(index_id).indexes = final_voxels;
    }
}

std::vector<int> ModelROI::getIndexes(unsigned int id)
{
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Indexes retrieval" << std::endl;
                break;
            }
        }
    }
    else
    {
        std::cerr << "Attempt to access Indexes from ROI on empty List- Class Model ROI" << std::endl;
    }

    return m_roi[index_id].indexes;

}

void ModelROI::setMultipleIndexes(std::vector<int> ids)
{
    bool error = false;
    //Clear indices from multiple ROI vector
    m_indexes_seed_concatenation.clear();
    m_indexes_and_concatenation.clear();
    m_indexes_not_concatenation.clear();
    m_number_fiter_not_concatenation_index.clear();
    m_number_fiter_and_concatenation_index.clear();

    for(unsigned int t = 0; t < ids.size(); t++)
    {
        unsigned int index_id = 0;
        if (m_roi.size() > 0)
        {
            error = false;
            while(ids[t] != (int)m_roi.at(index_id).id)
            {
                index_id++;
                if (index_id >= m_roi.size())
                {
                    std::cerr << "Couldn't find ROI with id " << ids[t] << " for Indexes retrieval" << std::endl;
                    error = true;
                    break;
                }
            }

            if(!error)
            {
                if(m_roi[index_id].type == roi_type::SEED)
                {
                    if(m_roi[index_id].plane == roi_plane::AXIAL_2D) // add a point (plane ID) to indicate the beginning of an axial ROI
                    {
                        m_indexes_seed_concatenation.push_back(-1);
                        m_indexes_seed_concatenation.push_back(-1);
                        m_indexes_seed_concatenation.push_back(-1);

                    } else if(m_roi[index_id].plane == roi_plane::CORONAL_2D) {  // add a point (plane ID) to indicate the beginning of a coronal ROI

                        m_indexes_seed_concatenation.push_back(-2);
                        m_indexes_seed_concatenation.push_back(-2);
                        m_indexes_seed_concatenation.push_back(-2);

                    } else if(m_roi[index_id].plane == roi_plane::SAGITTAL_2D) { // add a point (plane ID) to indicate the beginning of a sagittal ROI

                        m_indexes_seed_concatenation.push_back(-3);
                        m_indexes_seed_concatenation.push_back(-3);
                        m_indexes_seed_concatenation.push_back(-3);
                    }

                    for(unsigned int n = 0; n < m_roi[index_id].indexes.size(); n++)
                    {
                        m_indexes_seed_concatenation.push_back(m_roi[index_id].indexes[n]);
                    }
                }
                else if (m_roi[index_id].type == roi_type::NOT_FILTER)
                {
                    for(unsigned int n = 0; n < m_roi[index_id].indexes.size(); n++)
                    {
                        m_indexes_not_concatenation.push_back(m_roi[index_id].indexes[n]);
                    }
                    m_number_fiter_not_concatenation_index.push_back((m_roi[index_id].indexes.size()-1)/3);
                } else if (m_roi[index_id].type == roi_type::AND_FILTER)
                {
                    for(unsigned int n = 0; n < m_roi[index_id].indexes.size(); n++)
                    {
                        m_indexes_and_concatenation.push_back(m_roi[index_id].indexes[n]);
                    }
                    m_number_fiter_and_concatenation_index.push_back((m_roi[index_id].indexes.size()-1)/3);
                }
            }
        }
        else
        {
            std::cerr << "Attempt to access Indexes from ROI on empty List- Class Model ROI" << std::endl;
        }
    }


}

void ModelROI::multipleROIindexesComputed(std::vector<int> ids)
{
    m_multiple_roi_id.clear();
    for(unsigned int i = 0; i < ids.size(); i++)
    {
        m_multiple_roi_id.push_back(ids[i]);
    }
}

std::vector<int> ModelROI::getMultipleROIidsVisualization()
{
    return m_multiple_roi_id;
}

ModelROISetEvent ModelROI::setEnableRendering(bool enable)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setEnableRendering -->> A notification is in progress!" << std::endl;
    }

    m_enable_render = enable;
    if(enable == true)
        return ModelROISetEvent::RENDER_ROI;
    else
        return ModelROISetEvent::NOT_RENDER_ROI;
}

ModelROI::roi ModelROI::getROIbyID(unsigned short id)
{
    unsigned short index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if(index_id >= m_roi.size())
            {
                std::cerr << "ModelROI::getROIbyID -->> Can't Find ROI!" << std::endl;
                return roi();
            }
        }
    }
    return m_roi[index_id];
}

ModelROI::roi ModelROI::getROIbyIndex(unsigned short index)
{
    if (index < m_roi.size())
    {
        return m_roi[index];

    } else {
        std::cerr << "ModelROI::getROIbyIndex -->> Can't Find ROI!" << std::endl;
        return roi();
    }
}

ModelROISetEvent ModelROI::setCurrentActiveROIindex(int index)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setCurrentActiveROIindex -->> A notification is in progress!" << std::endl;
    }

    m_current_active_ROI_index = index;
    std::cout << "ModelROI::setCurrentActiveROIindex is: " << index << std::endl;
    return ModelROISetEvent::UPDATE_RENDER;
}

int ModelROI::getCurrentActiveROIindex()
{
    return m_current_active_ROI_index;
}

int ModelROI::getCurrentActiveROIid()
{
    return m_current_active_ROI_id;
}

ModelROISetEvent ModelROI::setCurrentActiveROIid(unsigned int id)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setCurrentActiveROIid -->> A notification is in progress!" << std::endl;
    }

    m_current_active_ROI_id = id;

    if(m_roi.size() != 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Setting Slice in ROI Model" << std::endl;
                return ModelROISetEvent::INDEX_ERROR;
            }
        }
        roi & desiredROI = m_roi.at(index_id);

        if(desiredROI.plane != ModelROI::NON_INITIALAZED)
            update2DSlice(desiredROI);
    }
    return ModelROISetEvent::UPDATE_RENDER;
}

void ModelROI::update2DSlice(roi &roi)
{
    Model2DSliceEvent event = Model2DSliceEvent::NONE;

    if(roi.plane == ModelROI::AXIAL_2D)
    {
        event = m_model2DSlice->setAxialSlice(roi.slice);
    }
    else if (roi.plane == ModelROI::SAGITTAL_2D)
    {
        event = m_model2DSlice->setSagittalSlice(roi.slice);
    }
    else if (roi.plane == ModelROI::CORONAL_2D)
    {
        event = m_model2DSlice->setCoronalSlice(roi.slice);
    }
    m_model2DSlice->notify(event);
}

unsigned short ModelROI::getNumberOfROIs()
{
    return m_roi.size();
}

int ModelROI::getLastRemovedROIindex()
{
    return m_last_removed_roi_index;
}

int ModelROI::getLastRemovedROIid()
{
    return m_last_removed_roi_id;
}

int ModelROI::new_id()
{
    return id_generator++;
}

std::vector<std::string> ModelROI::getROInameInformation()
{
    std::vector<std::string> roi_name_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        roi_name_information.push_back(m_roi[i].name);
    }

    return roi_name_information;
}

std::vector<int> ModelROI::getROItractIdInfo()
{
    std::vector<int> roi_tract_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        roi_tract_information.push_back(m_roi[i].tract_id);
    }

    return roi_tract_information;
}

std::string ModelROI::getROItractNameInfo(unsigned int id_roi)
{
    std::string tract_name;
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id_roi != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id_roi << " for Tract Name recovery" << std::endl;
                return tract_name;
            }
        }
    }

    tract_name = m_roi.at(index_id).tract_name;
    return tract_name;
}

std::vector<bool> ModelROI::getROIActivityInformation()
{
    std::vector<bool> roi_visibility_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        roi_visibility_information.push_back(m_roi[i].active);
    }

    return roi_visibility_information;
}

std::vector<int> ModelROI::deletedTract(int tract_id)
{
    std::vector<int> roi_ids_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(tract_id == m_roi[i].tract_id)
        {
            roi_ids_information.push_back(m_roi[i].id);
        }
    }
    return roi_ids_information;
}

std::vector<int> ModelROI::getActiveROIs()
{
    std::vector<int> roi_ids_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].active)
        {
            roi_ids_information.push_back(m_roi[i].id);
        }
    }
    return roi_ids_information;
}

std::vector<int> ModelROI::getActiveROIs(short tract_id)
{
    std::vector<int> roi_ids_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].active && m_roi[i].tract_id == tract_id)
        {
            roi_ids_information.push_back(m_roi[i].id);
        }
    }
    return roi_ids_information;
}

std::vector<int> ModelROI::getLoadedROIs()
{
    return m_roi_ids_loaded;
}

void ModelROI::setROIactivity(bool state, unsigned int id)
{
    unsigned int index_id = 0;
    if (m_roi.size() > 0)
    {
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for Activity set" << std::endl;
                return;
            }
        }
    }
    roi & desiredROI = m_roi.at(index_id);
    desiredROI.active = state;
}

void ModelROI::setROIactivityTractBased(bool state, int tract_id)
{
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].tract_id == tract_id)
        {
            m_roi[i].active = state;
        }
    }
}

std::string ModelROI::getROIname(unsigned int id)
{
    if(m_roi.size() != 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for recuperate ROI Name" << std::endl;
                return "";
            }
        }

        return m_roi.at(index_id).name;
    }
        return "";
}

unsigned int ModelROI::getSlice(unsigned int id)
{
    if(m_roi.size() != 0)
    {
        unsigned int index_id = 0;
        while(id != m_roi.at(index_id).id)
        {
            index_id++;
            if (index_id >= m_roi.size())
            {
                std::cerr << "Couldn't find ROI with id " << id << " for recuperate ROI Slice" << std::endl;
                return 100000;
            }
        }

    return m_roi[index_id].slice;

    }

    std::cerr << "Couldn't find ROI with id " << id << " for recuperate ROI Slice" << std::endl;
    return 100000;
}

std::vector<int> ModelROI::getROIidInformation()
{
    std::vector<int> roi_ids_information;
    for(unsigned int i = 0; i < m_roi.size(); i++)
    {
        roi_ids_information.push_back(m_roi[i].id);
    }

    return roi_ids_information;
}

bool ModelROI::getController_stroke_draw_activation() const
{
    return m_controller_stroke_draw_activation;
}

void ModelROI::setController_stroke_draw_activation(bool controller_stroke_draw_activation)
{
    m_controller_stroke_draw_activation = controller_stroke_draw_activation;
}

std::string ModelROI::save()
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    std::string f;
    f.append("ROI FILE\n");
    f.append("PATIENT_ID ");
    f.append(m_patient_id.c_str());
    f.append("\n");
    f.append("VOLUME_DIMENSION ");
    f.append(std::to_string(m_dimension[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[2]).c_str());
    f.append("\n");
    f.append("PIXEL_SPACING ");
    f.append(std::to_string(m_pixelSpacing[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[2]).c_str());
    f.append("\n");
    f.append("ID_Count ");
    f.append(std::to_string(id_generator).c_str());
    f.append("\n");
    //Get All elements of ROI and format to desired output
    f.append("NUMBER_OF_ROIS ");
    f.append(std::to_string(m_roi.size()).c_str());
    f.append("\n");

    for (unsigned int i = 0; i < m_roi.size(); i++)
    {
        f.append(m_roi[i].name.c_str());
        f.append("\n");
        f.append(std::to_string(m_roi[i].id).c_str());
        f.append(" ");
        f.append(std::to_string(m_roi[i].type).c_str());
        f.append(" ");
        f.append(std::to_string(m_roi[i].plane).c_str());
        f.append("\n");
        f.append(std::to_string(m_roi[i].points.size()).c_str());
        f.append("\n");
        for(unsigned int np = 0; np < m_roi[i].points.size(); np++)
        {
            f.append(std::to_string(m_roi[i].points[np].x).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].points[np].y).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].points[np].z).c_str());
            f.append(" ");
        }
        f.append("\n");
    }
    return f;
}

std::string ModelROI::save_session()
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    std::string f;
    f.append("ROI FILE SESSION\n");
    f.append("PATIENT_ID ");
    f.append(m_patient_id.c_str());
    f.append("\n");
    f.append("VOLUME_DIMENSION ");
    f.append(std::to_string(m_dimension[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[2]).c_str());
    f.append("\n");
    f.append("PIXEL_SPACING ");
    f.append(std::to_string(m_pixelSpacing[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[2]).c_str());
    f.append("\n");
    f.append("ID_Count ");
    f.append(std::to_string(id_generator).c_str());
    f.append("\n");
    //Get All elements of ROI and format to desired output
    f.append("NUMBER_OF_ROIS ");
    f.append(std::to_string(m_roi.size()).c_str());
    f.append("\n");

    for (unsigned int i = 0; i < m_roi.size(); i++)
    {
        f.append(m_roi[i].name.c_str());
        f.append("\n");
        f.append(std::to_string(m_roi[i].id).c_str());
        f.append(" ");
        f.append(std::to_string(m_roi[i].type).c_str());
        f.append(" ");
        f.append(std::to_string(m_roi[i].plane).c_str());
        f.append("\n");
        f.append(m_roi[i].tract_name.c_str());
        f.append(" ");
        f.append(std::to_string(m_roi[i].tract_id).c_str());
        f.append("\n");
        f.append(std::to_string(m_roi[i].points.size()).c_str());
        f.append("\n");
        for(unsigned int np = 0; np < m_roi[i].points.size(); np++)
        {
            f.append(std::to_string(m_roi[i].points[np].x).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].points[np].y).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].points[np].z).c_str());
            f.append(" ");
        }
        f.append("\n");
    }
    return f;
}

bool ModelROI::save_v2(int id_tract, std::string name_tract, std::string filename)
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    std::string f;
    f.append("ROI FILE v2\n");
    f.append("PATIENT_ID ");
    f.append(m_patient_id.c_str());
    f.append("\n");
    f.append("VOLUME_DIMENSION ");
    f.append(std::to_string(m_dimension[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_dimension[2]).c_str());
    f.append("\n");
    f.append("PIXEL_SPACING ");
    f.append(std::to_string(m_pixelSpacing[0]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[1]).c_str());
    f.append(" ");
    f.append(std::to_string(m_pixelSpacing[2]).c_str());
    f.append("\n");
    f.append("ID_Count ");
    f.append(std::to_string(id_generator).c_str());
    f.append("\n");
    f.append("TRACT_NAME ");
    f.append(name_tract.c_str());
    f.append("\n");
    f.append("TRACT_ID ");
    f.append(std::to_string(id_tract).c_str());
    f.append("\n");
    int number_of_rois = 0;
    for (unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].tract_id == id_tract)
            number_of_rois++;
    }

    //Save only ROI's that are associated with this particular tract
    f.append("NUMBER_OF_ROIS ");
    f.append(std::to_string(number_of_rois).c_str());
    f.append("\n");

    for (unsigned int i = 0; i < m_roi.size(); i++)
    {
        if(m_roi[i].tract_id == id_tract)
        {
            f.append(m_roi[i].name.c_str());
            f.append("\n");
            f.append(std::to_string(m_roi[i].id).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].type).c_str());
            f.append(" ");
            f.append(std::to_string(m_roi[i].plane).c_str());
            f.append("\n");
            f.append(std::to_string(m_roi[i].points.size()).c_str());
            f.append("\n");
            for(unsigned int np = 0; np < m_roi[i].points.size(); np++)
            {
                f.append(std::to_string(m_roi[i].points[np].x).c_str());
                f.append(" ");
                f.append(std::to_string(m_roi[i].points[np].y).c_str());
                f.append(" ");
                f.append(std::to_string(m_roi[i].points[np].z).c_str());
                f.append(" ");
            }
            f.append("\n");
        }
    }

    std::ofstream out(filename);

    if(out.is_open())
    {
        out << f;
        out.close();

    } else
        return false;

    return true;
}

ModelROISetEvent ModelROI::load(std::vector<std::string> file, std::string name_tract, int id_tract)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::load -->> A notification is in progress!" << std::endl;
    }


    ModelROISetEvent event = ModelROISetEvent::NONE;
    if(file[0].compare("ROI FILE") == 0)
    {
        event = loadROI(file);
    }
    else if(file[0].compare("ROI FILE v2") == 0)
    {
        event = loadROI_v2(file, id_tract);
    }
    else if(file[0].compare("ROI FILE SESSION") == 0)
    {
        event = loadROI_session(file);
    }

    std::vector<int> loaded_rois = getLoadedROIs();
    for(unsigned long int i = 0; i < loaded_rois.size(); i++)
        setTractROI(loaded_rois[i], name_tract, id_tract);

    ModelROISetEvent event_change = setModelChange(true);
    notify(event_change, nullptr);

    return event;
}

ModelROISetEvent ModelROI::loadROI(std::vector<std::string> file)
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    if(isNotifying())
    {
        std::cerr << "ModelROI::loadROI -->> A notification is in progress!" << std::endl;
    }

    std::cout << "Constructing Model Based on FILE" << std::endl;

    //Clear Loaded ROI id list
    m_roi_ids_loaded.clear();

    //Check if First Line is a ROI File
    if(file[0].compare("ROI FILE") != 0)
    {
        std::cerr << "Reading wrong File as ROI FILE" << std::endl;
        return ModelROISetEvent::NONE;
    }

    //Patient ID
    std::vector<std::string> string_part;
    splitString(file[1], string_part, ' ');
    if(string_part.size() > 1)
    {
        //Get patient ID if exist one
        std::cout << "P ID: " << string_part[1] << std::endl;
    }
    string_part.clear();

    //Volume Dimension
    splitString(file[2], string_part, ' ');
    unsigned int vol_dim[3];
    vol_dim[0] = std::stoi(string_part[1]);
    vol_dim[1] = std::stoi(string_part[2]);
    vol_dim[2] = std::stoi(string_part[3]);
    if(m_dimension[0] == vol_dim[0] && m_dimension[1] == vol_dim[1] && m_dimension[2] == vol_dim[2])
    {
        std::cout << "Vol DIM 1: " << vol_dim[0] << std::endl;
        std::cout << "Vol DIM 2: " << vol_dim[1] << std::endl;
        std::cout << "Vol DIM 3: " << vol_dim[2] << std::endl;
    }
    else
    {
        if(!m_mainWindow->showQuestionMessageBox("Wrong Volume Dimension", "The ROI volume dimension reference you are loading does not match the open volume. Do you wanto to continue?"))
        {
            return ModelROISetEvent::NONE;
        }
    }
    string_part.clear();

    //Pixel Spacing
    splitString(file[3], string_part, ' ');
    float pixel_space[3];

    std::cout << "Pixel Space String" << string_part[1] << " " << string_part[2] << " " << string_part[3] << std::endl;

    std::stringstream string_s1;
    string_s1 << string_part[1];
    string_s1 >> pixel_space[0];
    string_s1.clear();
    std::stringstream string_s2;
    string_s2 << string_part[2];
    string_s2 >> pixel_space[1];
    string_s2.clear();
    std::stringstream string_s3;
    string_s3 << string_part[3];
    string_s3 >> pixel_space[2];
    string_s3.clear();

    std::cout << "Pixel  Spacing 1: " << pixel_space[0] << std::endl;
    std::cout << "Pixel  Spacing 2: " << pixel_space[1] << std::endl;
    std::cout << "Pixel  Spacing 3: " << pixel_space[2] << std::endl;

    string_part.clear();

    float maxPhySlice = MAX(MAX(vol_dim[0] * pixel_space[0], vol_dim[1] * pixel_space[1]), vol_dim[2] * pixel_space[2]);

    float phyScale[3];
    float scale_factors[4];

    phyScale[0] = (pixel_space[0]) / maxPhySlice;
    phyScale[1] = (pixel_space[1]) / maxPhySlice;
    phyScale[2] = (pixel_space[2]) / maxPhySlice;

    scale_factors[0] = (vol_dim[0] * pixel_space[0]) / maxPhySlice;
    scale_factors[1] = (vol_dim[1] * pixel_space[1]) / maxPhySlice;
    scale_factors[2] = (vol_dim[2] * pixel_space[2]) / maxPhySlice;
    scale_factors[3] = 1.f;

    //ID Count
    splitString(file[4], string_part, ' ');
    bool ignore_id_file = false;
    int id_count = std::stoi(string_part[1]);
    if(id_generator == 0)
    {
        //Update ID generator
        id_generator = id_count;
    }
    else
    {
        ignore_id_file = true;
    }
    std::cout << "ID Count " << id_count << std::endl;

    //Number of ROIs

    string_part.clear();

    splitString(file[5], string_part, ' ');

    int n_rois = std::stoi(string_part[1]);
    std::cout << "Number of ROIs: " << n_rois <<std::endl;

    unsigned int id_roi;
    roi_type type_roi;
    roi_plane plane_roi;
    std::string name_roi;
    int number_points;
    std::vector<glm::vec3> points_roi;
    glm::vec3 point_aux;
    for(int i = 0; i < n_rois; i++)
    {
        name_roi = (file[6+i*4]);
        std::cout << "Name ROI: " << name_roi << std::endl;
        string_part.clear();

        //ROI properties
        splitString(file[7+i*4], string_part, ' ');
        if(!ignore_id_file)
        {
            id_roi = std::stoi(string_part[0]);
        }
        else
        {
            id_roi = new_id();
        }
        type_roi = static_cast<roi_type>(std::stoi(string_part[1]));
        plane_roi = static_cast<roi_plane>(std::stoi(string_part[2]));

        std::cout << "ID ROI: "<< id_roi << std::endl;
        std::cout << "Type ROI: "<< type_roi << std::endl;
        std::cout << "Plane ROI:" << plane_roi << std::endl;

        ModelROISetEvent event = newROI(type_roi, plane_roi, id_roi);
        setNameROI(id_roi, name_roi);
        notify(event, nullptr);

        m_roi_ids_loaded.push_back(id_roi);

        number_points = std::stoi(file[8+i*4]);
        std::cout << "Number of Points: " << number_points << std::endl;

        std::stringstream ss;
        string_part.clear();
        splitString(file[9+i*4], string_part, ' ');
        ModelROISetEvent event_point;
        for(int n = 0; n < number_points; n++){
            replaceString(string_part[n*3], ",", ".");
            ss << string_part[n*3].c_str();
            ss >> point_aux.x;
            ss.clear();

            replaceString(string_part[n*3+1], ",", ".");
            ss << string_part[n*3+1].c_str();
            ss >> point_aux.y;
            ss.clear();

            replaceString(string_part[n*3+2], ",", ".");
            ss << string_part[n*3+2].c_str();
            ss >> point_aux.z;
            ss.clear();

            event_point = addPoint(id_roi, point_aux);
            notify(event_point, nullptr);

            points_roi.push_back(point_aux);

            if(n == 0)
            {
                //Compute and change Slice accordingly
                if(plane_roi == roi_plane::AXIAL_2D)
                {
                    float aux = (point_aux.z +(scale_factors[2]))/ (2.0 * phyScale[2]);
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Axial change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);

                }
                else if(plane_roi == roi_plane::SAGITTAL_2D)
                {
                    float aux = ((point_aux.x + (scale_factors[0]))/ (2.0 * phyScale[0]));
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Sagittal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
                else if(plane_roi == roi_plane::CORONAL_2D)
                {
                    float aux = ((scale_factors[1] - point_aux.y)/ (2.0 * phyScale[1]));
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Coronal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
            }
        }

    }

    //Let UI Controller know that new ROI's have been loaded
    return ModelROISetEvent::ROI_LOADED;
}

ModelROISetEvent ModelROI::loadROI_session(std::vector<std::string> file)
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    if(isNotifying())
    {
        std::cerr << "ModelROI::loadROI_session -->> A notification is in progress!" << std::endl;
    }

    std::cout << "Constructing Model Based on Session ROI" << std::endl;
    std::vector<std::string> tract_names;

    //Clear loaded ROI's id list
    m_roi_ids_loaded.clear();

    //Check if First Line is a ROI File
    if(file[0].compare("ROI FILE SESSION") != 0)
    {
        std::cerr << "Reading wrong File as ROI FILE" << std::endl;
        return ModelROISetEvent::NONE;
    }

    //Patient ID
    std::vector<std::string> string_part;
    splitString(file[1], string_part, ' ');
    if(string_part.size() > 1)
    {
        //Get patient ID if exist one
        std::cout << "P ID: " << string_part[1] << std::endl;
    }
    string_part.clear();

    //Volume Dimension
    splitString(file[2], string_part, ' ');
    unsigned int vol_dim[3];
    vol_dim[0] = std::stoi(string_part[1]);
    vol_dim[1] = std::stoi(string_part[2]);
    vol_dim[2] = std::stoi(string_part[3]);
    if(m_dimension[0] == vol_dim[0] && m_dimension[1] == vol_dim[1] && m_dimension[2] == vol_dim[2])
    {
        std::cout << "Vol DIM 1: " << vol_dim[0] << std::endl;
        std::cout << "Vol DIM 2: " << vol_dim[1] << std::endl;
        std::cout << "Vol DIM 3: " << vol_dim[2] << std::endl;
    }
    else
    {
        if(!m_mainWindow->showQuestionMessageBox("Wrong Volume Dimension", "The ROI volume dimension reference you are loading does not match the open volume. Do you wanto to continue?"))
        {
            return ModelROISetEvent::NONE;
        }
    }
    string_part.clear();

    //Pixel Spacing
    splitString(file[3], string_part, ' ');
    float pixel_space[3];

    std::cout << "Pixel Space String" << string_part[1] << " " << string_part[2] << " " << string_part[3] << std::endl;

    std::stringstream string_s1;
    string_s1 << string_part[1];
    string_s1 >> pixel_space[0];
    string_s1.clear();
    std::stringstream string_s2;
    string_s2 << string_part[2];
    string_s2 >> pixel_space[1];
    string_s2.clear();
    std::stringstream string_s3;
    string_s3 << string_part[3];
    string_s3 >> pixel_space[2];
    string_s3.clear();

    std::cout << "Pixel  Spacing 1: " << pixel_space[0] << std::endl;
    std::cout << "Pixel  Spacing 2: " << pixel_space[1] << std::endl;
    std::cout << "Pixel  Spacing 3: " << pixel_space[2] << std::endl;

    string_part.clear();

    float maxPhySlice = MAX(MAX(vol_dim[0] * pixel_space[0], vol_dim[1] * pixel_space[1]), vol_dim[2] * pixel_space[2]);

    float phyScale[3];
    float scale_factors[4];

    phyScale[0] = (pixel_space[0]) / maxPhySlice;
    phyScale[1] = (pixel_space[1]) / maxPhySlice;
    phyScale[2] = (pixel_space[2]) / maxPhySlice;

    scale_factors[0] = (vol_dim[0] * pixel_space[0]) / maxPhySlice;
    scale_factors[1] = (vol_dim[1] * pixel_space[1]) / maxPhySlice;
    scale_factors[2] = (vol_dim[2] * pixel_space[2]) / maxPhySlice;
    scale_factors[3] = 1.f;

    //ID Count
    splitString(file[4], string_part, ' ');
    bool ignore_id_file = false;
    int id_count = std::stoi(string_part[1]);
    if(id_generator == 0)
    {
        //Update ID generator
        id_generator = id_count;
    }
    else
    {
        ignore_id_file = true;
    }
    std::cout << "ID Count " << id_count << std::endl;

    //Number of ROIs

    string_part.clear();

    splitString(file[5], string_part, ' ');

    int n_rois = std::stoi(string_part[1]);
    std::cout << "Number of ROIs: " << n_rois <<std::endl;

    unsigned int id_roi;
    roi_type type_roi;
    roi_plane plane_roi;
    std::string name_roi;
    std::string name_tract;
    int id_tract;
    int number_points;
    std::vector<glm::vec3> points_roi;
    glm::vec3 point_aux;
    for(int i = 0; i < n_rois; i++)
    {
        name_roi = (file[6+i*4]);
        std::cout << "Name ROI: " << name_roi << std::endl;
        string_part.clear();

        //ROI properties
        splitString(file[7+i*4], string_part, ' ');
        if(!ignore_id_file)
        {
            id_roi = std::stoi(string_part[0]);
        }
        else
        {
            id_roi = new_id();
        }
        type_roi = static_cast<roi_type>(std::stoi(string_part[1]));
        plane_roi = static_cast<roi_plane>(std::stoi(string_part[2]));
        string_part.clear();

        std::cout << "ID ROI: "<< id_roi << std::endl;
        std::cout << "Type ROI: "<< type_roi << std::endl;
        std::cout << "Plane ROI:" << plane_roi << std::endl;

        //Tract Properties
        splitString(file[8+i*4], string_part, ' ');
        name_tract = string_part[0];
        id_tract = std::stoi(string_part[1]);

        tract_names.push_back(name_tract);

        std::cout << "Name Tract: "<< name_tract << std::endl;
        std::cout << "ID Tract  : "<< id_tract << std::endl;


        ModelROISetEvent event = newROI(type_roi, plane_roi, id_roi);
        setNameROI(id_roi, name_roi);
        setTractROI(id_roi, name_tract, id_tract);
        notify(event, nullptr);

        m_roi_ids_loaded.push_back(id_roi);

        number_points = std::stoi(file[9+i*4]);
        std::cout << "Number of Points: " << number_points << std::endl;

        std::stringstream ss;
        string_part.clear();
        splitString(file[10+i*4], string_part, ' ');
        ModelROISetEvent event_point;
        for(int n = 0; n < number_points; n++){
            replaceString(string_part[n*3], ",", ".");
            ss << string_part[n*3].c_str();
            ss >> point_aux.x;
            ss.clear();

            replaceString(string_part[n*3+1], ",", ".");
            ss << string_part[n*3+1].c_str();
            ss >> point_aux.y;
            ss.clear();

            replaceString(string_part[n*3+2], ",", ".");
            ss << string_part[n*3+2].c_str();
            ss >> point_aux.z;
            ss.clear();

            event_point = addPoint(id_roi, point_aux);
            notify(event_point, nullptr);

            points_roi.push_back(point_aux);

            if(n == 0)
            {
                //Compute and change Slice accordingly
                if(plane_roi == roi_plane::AXIAL_2D)
                {
                    float aux = (point_aux.z +(scale_factors[2]))/ (2.0 * phyScale[2]);
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Axial change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);

                }
                else if(plane_roi == roi_plane::SAGITTAL_2D)
                {
                    float aux = ((point_aux.x + (scale_factors[0]))/ (2.0 * phyScale[0]));
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Sagittal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
                else if(plane_roi == roi_plane::CORONAL_2D)
                {
                    float aux = ((scale_factors[1] - point_aux.y)/ (2.0 * phyScale[1]));
                    unsigned int slice = (unsigned int) aux;
                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    m_roi[index_id].slice = slice;
                    std::cout << "Slice Coronal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
            }
        }

    }

    //Remove Duplicates from Tract List
    for(unsigned long int i = 0; i < tract_names.size(); i++)
    {
        std::string str_ref = tract_names[i];
        for(unsigned long int j = i+1; j < tract_names.size(); j++)
        {
            if(str_ref.compare(tract_names[j]) == 0)
                tract_names.erase(tract_names.begin()+j);
        }
    }

    std::cout << "Final List Tract Names on Load Session" << std::endl;
    for(unsigned long int i = 0; i < tract_names.size(); i++)
    {
        std::cout << " Tract : " << tract_names[i] << std::endl;
    }

    //Let UI Controller know that new ROI's have been loaded
    return ModelROISetEvent::ROI_LOADED;
}

ModelROISetEvent ModelROI::loadROI_v2(std::vector<std::string> file, int new_id_tract)
{
    //Set the global locale in the C (Classic) locale (Ting, 05/07/2024)
    std::locale::global(std::locale("C"));

    if(isNotifying())
    {
        std::cerr << "ModelROI::loadROI_v2 -->> A notification is in progress!" << std::endl;
    }

    std::cout << "Loading ROI File v2 - Only one tract " << std::endl;
    std::cout << std::endl;

    std::vector<std::string> tract_names;
    std::string name_tract;

    //Clear Loaded ROI's list
    m_roi_ids_loaded.clear();

    //Check if First Line is a ROI File
    if(file[0].compare("ROI FILE v2") != 0)
    {
        std::cerr << "Reading wrong File as ROI FILE" << std::endl;
        return ModelROISetEvent::NONE;
    }

    //Patient ID
    std::vector<std::string> string_part;
    splitString(file[1], string_part, ' ');
    if(string_part.size() > 1)
    {
        //Get patient ID if exist one
        std::cout << "P ID: " << string_part[1] << std::endl;
    }
    string_part.clear();

    //Volume Dimension
    splitString(file[2], string_part, ' ');
    unsigned int vol_dim[3];
    vol_dim[0] = std::stoi(string_part[1]);
    vol_dim[1] = std::stoi(string_part[2]);
    vol_dim[2] = std::stoi(string_part[3]);
    if(m_dimension[0] == vol_dim[0] && m_dimension[1] == vol_dim[1] && m_dimension[2] == vol_dim[2])
    {
        std::cout << "Vol DIM 1: " << vol_dim[0] << std::endl;
        std::cout << "Vol DIM 2: " << vol_dim[1] << std::endl;
        std::cout << "Vol DIM 3: " << vol_dim[2] << std::endl;
    }
    else
    {
        if(!m_mainWindow->showQuestionMessageBox("Wrong Volume Dimension", "The ROI volume dimension reference you are loading does not match the open volume. Do you wanto to continue?"))
        {
            return ModelROISetEvent::NONE;
        }
    }
    string_part.clear();

    //Pixel Spacing
    splitString(file[3], string_part, ' ');
    float pixel_space[3];

    std::cout << "Pixel Space String" << string_part[1] << " " << string_part[2] << " " << string_part[3] << std::endl;

    std::stringstream string_s1;
    string_s1 << string_part[1];
    string_s1 >> pixel_space[0];
    string_s1.clear();
    std::stringstream string_s2;
    string_s2 << string_part[2];
    string_s2 >> pixel_space[1];
    string_s2.clear();
    std::stringstream string_s3;
    string_s3 << string_part[3];
    string_s3 >> pixel_space[2];
    string_s3.clear();

    std::cout << "Pixel  Spacing 1: " << pixel_space[0] << std::endl;
    std::cout << "Pixel  Spacing 2: " << pixel_space[1] << std::endl;
    std::cout << "Pixel  Spacing 3: " << pixel_space[2] << std::endl;

    string_part.clear();

    float maxPhySlice = MAX(MAX(vol_dim[0] * pixel_space[0], vol_dim[1] * pixel_space[1]), vol_dim[2] * pixel_space[2]);

    float phyScale[3];
    float scale_factors[4];

    phyScale[0] = (pixel_space[0]) / maxPhySlice;
    phyScale[1] = (pixel_space[1]) / maxPhySlice;
    phyScale[2] = (pixel_space[2]) / maxPhySlice;

    scale_factors[0] = (vol_dim[0] * pixel_space[0]) / maxPhySlice;
    scale_factors[1] = (vol_dim[1] * pixel_space[1]) / maxPhySlice;
    scale_factors[2] = (vol_dim[2] * pixel_space[2]) / maxPhySlice;
    scale_factors[3] = 1.f;

    //ID Count
    splitString(file[4], string_part, ' ');
    int id_count = std::stoi(string_part[1]);
    bool ignore_id_file = false;
    if(id_generator == 0)
    {
        //Update ID generator
        id_generator = id_count;
    }
    else
    {
        ignore_id_file = true;
    }
    string_part.clear();
    std::cout << "ID Count " << id_count << std::endl;


    //Tract Name
    splitString(file[5], string_part, ' ');
    name_tract = string_part[1];
    //Add single tract to list
    tract_names.push_back(name_tract);
    string_part.clear();

    std::cout << "Tract Name " << name_tract << std::endl;

    //Tract ID
    splitString(file[6], string_part, ' ');
    int id_tract;
    id_tract = std::stoi(string_part[1]);
    string_part.clear();

    std::cout << " Old tract ID " << id_tract << ", new track ID " << new_id_tract << std::endl;
    id_tract = new_id_tract;

    //Number of ROIs
    splitString(file[7], string_part, ' ');

    int n_rois = std::stoi(string_part[1]);
    std::cout << "Number of ROIs: " << n_rois <<std::endl;

    unsigned int id_roi;
    roi_type type_roi;
    roi_plane plane_roi;
    std::string name_roi;
    int number_points;
    std::vector<glm::vec3> points_roi;
    glm::vec3 point_aux;
    for(int i = 0; i < n_rois; i++)
    {
        name_roi = (file[8+i*4]);
        std::cout << "Name ROI: " << name_roi << std::endl;
        string_part.clear();

        //ROI properties
        splitString(file[9+i*4], string_part, ' ');
        if(!ignore_id_file)
        {
            id_roi = std::stoi(string_part[0]);
        }
        else
        {
            id_roi = new_id();
        }
        type_roi = static_cast<roi_type>(std::stoi(string_part[1]));
        plane_roi = static_cast<roi_plane>(std::stoi(string_part[2]));

        std::cout << "ID ROI: "<< id_roi << std::endl;
        std::cout << "Type ROI: "<< type_roi << std::endl;
        std::cout << "Plane ROI:" << plane_roi << std::endl;

        ModelROISetEvent event = newROI(type_roi, plane_roi, id_roi);
        setNameROI(id_roi, name_roi);
        setTractROI(id_roi, name_tract, id_tract);
        setROIactivity(true, id_roi);
        notify(event, nullptr);

        m_roi_ids_loaded.push_back(id_roi);

        number_points = std::stoi(file[10+i*4]);
        std::cout << "Number of Points: " << number_points << std::endl;

        std::stringstream ss;
        string_part.clear();
        splitString(file[11+i*4], string_part, ' ');
        ModelROISetEvent event_point;
        for(int n = 0; n < number_points; n++){
            replaceString(string_part[n*3], ",", ".");  //Ting: Locale C  (06/07/2024)
            ss << string_part[n*3].c_str();
            ss >> point_aux.x;
            ss.clear();
            //point_aux.x = std::stof(string_part[n*3].c_str(), 0);
            replaceString(string_part[n*3+1], ",", ".");     //Ting: Locale C  (06/07/2024)
            ss << string_part[n*3+1].c_str();
            ss >> point_aux.y;
            ss.clear();
            //point_aux.y = std::stof(string_part[n*3+1].c_str(), 0);
            replaceString(string_part[n*3+2], ",", ".");     //Ting: Locale C  (06/07/2024)
            ss << string_part[n*3+2].c_str();
            ss >> point_aux.z;
            ss.clear();

            unsigned int slice;

            if(n == 0)
            {
                //Compute and change Slice accordingly
                if(plane_roi == roi_plane::AXIAL_2D)
                {
                    float aux = (point_aux.z +(scale_factors[2]))/ (2.0 * phyScale[2]);
//                    float aux = (point_aux.z*maxPhySlice +(vol_dim[2] * pixel_space[2]))/ (2.0 * pixel_space[2]);

//                  unsigned int slice = (unsigned int) aux;     //Ting: working around truncation error (06/07/2024)

                    if (aux >= (unsigned int)aux + 0.5)
                         slice = (unsigned int) aux + 1;
                    else
                         slice = (unsigned int) aux;

                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    //Ting: readjust point_aux.z
                    point_aux.z = slice * 2.0 * phyScale[2] - scale_factors[2];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);

                    m_roi[index_id].slice = slice;
                    update2DSlice(m_roi[index_id]);
                    std::cout << "Slice Axial change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);

                }
                else if(plane_roi == roi_plane::SAGITTAL_2D)
                {
                    float aux = ((point_aux.x + (scale_factors[0]))/ (2.0 * phyScale[0]));
//                    float aux = (point_aux.x*maxPhySlice +(vol_dim[0] * pixel_space[0]))/ (2.0 * pixel_space[0]);

//                  unsigned int slice = (unsigned int) aux;           //Ting: working around truncation error (06/07/2024)
                    if (aux >= (unsigned int)aux + 0.5)
                        slice = (unsigned int) aux + 1;
                    else
                        slice = (unsigned int) aux;

                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    //Ting: readjust point_aux.x
                    point_aux.x = slice * 2.0 * phyScale[0] - scale_factors[0];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);

                    m_roi[index_id].slice = slice;
                    update2DSlice(m_roi[index_id]);
                    std::cout << "Slice Sagittal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
                else if(plane_roi == roi_plane::CORONAL_2D)
                {
                    float aux = ((scale_factors[1] - point_aux.y)/ (2.0 * phyScale[1]));
//                    float aux = ((vol_dim[1] * pixel_space[1]) - (point_aux.y * maxPhySlice))/ (2.0 * pixel_space[1]);

//                  unsigned int slice = (unsigned int) aux;

                    if (aux >= (unsigned int)aux + 0.5)           //Ting: working round truncation error (06/07/2024)
                        slice = (unsigned int) aux + 1;
                    else
                        slice = (unsigned int) aux;

                    unsigned int index_id = 0;
                    while(id_roi != m_roi.at(index_id).id)
                    {
                        index_id++;
                        if (index_id >= m_roi.size())
                        {
                            std::cerr << "Couldn't find ROI with id " << id_roi << " for change Slice value on ROI Load" << std::endl;
                        }
                    }
                    //Ting: readjust point_aux.y
                    point_aux.y = scale_factors[1] - slice * 2.0 * phyScale[1];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);

                    m_roi[index_id].slice = slice;
                    update2DSlice(m_roi[index_id]);
                    std::cout << "Slice Coronal change to: " << slice <<std::endl;
                    notify(ModelROISetEvent::ROI_SLICE_CHANGE);
                }
            } else if (n > 0) {
                //Ting: Becauce the slice is rounded, the coordinates must also be rounded (06/07/2024)
                if(plane_roi == roi_plane::AXIAL_2D)
                {
                    //Ting: readjust point_aux.z
                    point_aux.z = slice * 2.0 * phyScale[2] - scale_factors[2];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);
                }
                else if(plane_roi == roi_plane::SAGITTAL_2D)
                {
                   //Ting: readjust point_aux.x
                    point_aux.x = slice * 2.0 * phyScale[0] - scale_factors[0];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);
                }
                else if(plane_roi == roi_plane::CORONAL_2D)
                {
                    //Ting: readjust point_aux.y
                    point_aux.y = scale_factors[1] - slice * 2.0 * phyScale[1];
                    event_point = addPoint(id_roi, point_aux);
                    notify(event_point, nullptr);
                    points_roi.push_back(point_aux);
                }
            }
        }

    }

    //Let UI Controller know that new ROI's have been loaded
    return ModelROISetEvent::ROI_LOADED;
}

size_t ModelROI::splitString(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

bool ModelROI::replaceString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool ModelROI::isEnabled() const
{
    return m_isEnabled;
}

ModelROISetEvent ModelROI::setModelChange(bool change_model)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setModelChange -->> A notification is in progress!" << std::endl;
    }

    m_model_changed = change_model;
    return ModelROISetEvent::ROI_MODEL_CHANGED;
}

ModelROISetEvent ModelROI::setEnabled(bool state)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::setEnabled -->> A notification is in progress!" << std::endl;
    }

    if(m_isEnabled == state)
        return ModelROISetEvent::NONE;

    m_isEnabled = state;

    if(m_stateManager != nullptr)
    {
        if(m_isEnabled)
            m_stateManager->activateFeature("ROI", m_roiStateChangedHandlerSlotID);
        else
            m_stateManager->deactivateFeature("ROI", m_roiStateChangedHandlerSlotID);
    }

    return ModelROISetEvent::ROI_STATE_CHANGED;
}

void ModelROI::setStateManager(StateManager *state_manager)
{
    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("ROI", m_roiStateChangedHandlerSlotID);
        m_stateManager->removeFeature("ROI");
    }

    m_stateManager = state_manager;

    if(m_stateManager->addStateWithRestrictions("ROI", std::vector<std::string>{"MULTIPLANAR"}))
    {
        m_roiStateChangedHandlerSlotID = m_stateManager->subscribe("ROI", CREATE_SLOT(this, &ModelROI::handleROIStateChanged));

    }
    else
    {
        m_roiStateChangedHandlerSlotID = m_stateManager->subscribe("ROI", CREATE_SLOT(this, &ModelROI::handleROIStateChanged));
        m_isEnabled = m_stateManager->isActivated("ROI");
    }

}

void ModelROI::handleROIStateChanged(std::string , FeatureState state)
{
    if(isNotifying())
    {
        std::cerr << "ModelROI::handleROIStateChanged -->> A notification is in progress!" << std::endl;
    }

    bool is_activated;

    if(state == FeatureState::ACTIVATED)
        is_activated = true;
    else
        is_activated = false;

    if(m_isEnabled != is_activated)
    {
        m_isEnabled = is_activated;
        notify(ModelROISetEvent::ROI_STATE_CHANGED, nullptr);
    }
}
