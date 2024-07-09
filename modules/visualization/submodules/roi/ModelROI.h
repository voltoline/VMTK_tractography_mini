#ifndef MODELROI_H
#define MODELROI_H

//#include "ModelSubject.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <QString>
#include <string>
#include <glm/glm.hpp>
#include <sstream>
#ifdef usinglib_cursor
#include "internal_libraries/Cursor/include/ModelCursor.h"
#else
#include "modules/visualization/submodules/cursor/ModelCursor.h"
#endif
#include "core/MainWindow.h"

class Model2DTransformation;
class EventHandler2DTransformationToCursor;

enum class ModelROISetEvent
{
    NONE,
    ROI_ADDED,
    POINT_ADDED,
    ROI_CREATED,
    ROI_ENDED,
    CHANGE_TYPE_ROI,
    RENDER_ROI,
    NOT_RENDER_ROI,
    UPDATE_RENDER,
    ROI_REMOVED,
    LAST_ROI_REMOVED,
    ROI_PLANE_CHANGE,
    INDEX_ERROR,
    ROI_STATE_CHANGED,
    ROI_LOADED,
    ROI_SLICE_CHANGE,
    ROI_MODEL_CHANGED,
    ROI_EDITION
};

class ModelROI: public ModelSubject<ModelROI, ModelROISetEvent>
{
public:

    enum roi_type {
        SEED,
        NOT_FILTER,
        AND_FILTER,
    };

    enum roi_plane {
        NON_INITIALAZED,
        SAGITTAL_2D,
        CORONAL_2D,
        AXIAL_2D,
        MULTIPLANAR_3D,
    };

    typedef struct {
        unsigned int id;   //changed from int to unsigned int by Ting (29/04/2022)
        std::string name;
        int tract_id;
        std::string tract_name;
        unsigned int slice;
        roi_type type;
        roi_plane plane;
        std::vector<glm::vec3> points;
        std::vector<int> indexes;
        bool active;
    } roi;

    ModelROI();
    ~ModelROI();
    ModelROISetEvent newROI(roi_type type, std::string tract_name, int tract_id);
    ModelROISetEvent addROI(roi new_roi);
    ModelROISetEvent removeROI(unsigned int id, bool tract_removed);
    ModelROISetEvent addPoint(unsigned int id, glm::vec3 point);
    ModelROISetEvent endROI(unsigned int id);
    ModelROISetEvent setType(unsigned int id, roi_type type);
    ModelROISetEvent setEnableRendering(bool enable);
    int getTractID(unsigned int roi_id);

    ModelROISetEvent setROIEditionActivated(bool state);

    roi getROIbyID(unsigned short id);
    roi getROIbyIndex(unsigned short index);

    ModelROISetEvent setPlane(unsigned int id, roi_plane plane);
    roi_plane getPlane(unsigned int id);

    void setDataAcquisition(unsigned short dim_x, unsigned short dim_y, unsigned short dim_z,
                            double pixelSpacing_x, double pixelSpacing_y, double pixelSpacing_z, std::string id);
    void setMainWindow(MainWindow * main);
    void computeIndexes(unsigned int id);
    void multipleROIindexesComputed(std::vector<int> ids);
    std::vector<glm::vec3> getROIpoints(unsigned int id);

    std::vector<int> bresenham3DAlgorithm(int x0, int y0, int z0, int x1, int y1, int z1, bool debug);
    void computeCentroid3D(std::vector<int> p, int * centroid);
    void computeCentroid2D(std::vector<int> pos, double * centroid);
    int findInteriorVoxels(std::vector<int> p, int x, int y, int z, std::vector<int> &indexes, bool debug);
    void floodFill(std::vector<std::vector<int>> &color, int x, int y, int max_x, int max_y);
    void get2DPointsOf3DPlane(std::vector<int> in_3D, std::vector<int> &out_2D, int *plane);

    ModelROISetEvent setCurrentActiveROIindex(int index);
    ModelROISetEvent setCurrentActiveROIid(unsigned int id);
    int getCurrentActiveROIindex();
    int getCurrentActiveROIid();
    void setNameROI(unsigned int id, std::string name);
    void setTractROI(unsigned int id, std::string name_tract, int id_tract);

    std::vector<int> getROIidsFromTractId(int tract_id);
    std::string getROItractNameInfo(unsigned int id_roi);

    std::vector<int> getIndexes(unsigned int id);
    void setMultipleIndexes(std::vector<int> ids);
    std::vector<int> getMultipleROIidsVisualization();
    unsigned short getNumberOfROIs();
    int getLastRemovedROIindex();
    int getLastRemovedROIid();

    glm::vec3 getLastPointAdded();

    roi_type getType(unsigned int id);
    unsigned int getSlice(unsigned int id);

    void setModel2DSlice(Model2DSlice *model2DSlice);

    ModelCursor *getModelCursor() const;
    glm::vec3 getMousePosition3D();
    glm::vec3 getMousePositionAxial();
    glm::vec3 getMousePositionCoronal();
    glm::vec3 getMousePositionSagittal();

    std::vector<int> getIndexes_seed_concatenation() const;
    std::vector<int> getIndexes_not_concatenation() const;
    std::vector<int> getIndexes_and_concatenation() const;

    std::vector<int> getNumberFilterAND() const;
    std::vector<int> getNumberFilterNOT() const;

    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

    std::string save();
    std::string save_session();
    bool save_v2(int id_tract, std::string name_tract, std::string filename);
    ModelROISetEvent load(std::vector<std::string> file, std::string name_tract, int id_tract);

    ModelROISetEvent loadROI(std::vector<std::string> file);
    ModelROISetEvent loadROI_v2(std::vector<std::string> file, int new_id_tract);
    ModelROISetEvent loadROI_session(std::vector<std::string> file);

    std::vector<std::string> getROInameInformation();
    std::vector<int> getROIidInformation();
    std::vector<int> getROItractIdInfo();
    std::vector<int> deletedTract(int tract_id);

    bool getController_stroke_draw_activation() const;
    void setController_stroke_draw_activation(bool controller_stroke_draw_activation);

    std::string getROIname(unsigned int id);

    std::vector<int> getActiveROIs();
    std::vector<int> getActiveROIs(short tract_id);
    std::vector<int>  getLoadedROIs();
    void setROIactivity(bool state, unsigned int id);
    void setROIactivityTractBased(bool state, int tract_id);
    std::vector<bool> getROIActivityInformation();

    void handleROIStateChanged(std::string feature_name, FeatureState state);
    void setStateManager(StateManager *state_manager);
    ModelROISetEvent setEnabled(bool state);
    bool isEnabled() const;

    ModelROISetEvent setModelChange(bool change_model);

    MainWindow * getMainWindow();

    bool getRoiEditionActivated() const;

    void setCurrentEditableTractID(int currentEditableTractID);

private:

    std::vector<roi> m_roi;     //List of ROI

    double m_real_dimension[3];
    unsigned int m_dimension[3];
    double m_maxDimension;
    double m_real_maxDimension;
    double m_pixelSpacing[3];
    float m_fScaleFactors[4];
    unsigned int id_generator;
    std::string m_patient_id;

    glm::vec3 m_last_point_added;
    int m_current_active_ROI_index;
    int m_current_active_ROI_id;
    int m_last_removed_roi_index;
    int m_last_removed_roi_id;
    std::vector<int> m_multiple_roi_id;
    std::vector<int> m_indexes_seed_concatenation;
    std::vector<int> m_indexes_not_concatenation;
    std::vector<int> m_number_fiter_not_concatenation_index;
    std::vector<int> m_indexes_and_concatenation;
    std::vector<int> m_number_fiter_and_concatenation_index;

    std::vector<int> m_roi_ids_loaded;

//    int m_focus; //1 - axial, 2 - Sagittal, 3- Coronal
    bool m_enable_render;
    bool m_enable_free_render;
    bool m_controller_stroke_draw_activation;
    //Used to inform interface of a new state of the ROI model
    bool m_model_changed;

    EventHandler2DTransformationToCursor *m_eventHandlerCursor;
    ModelCursor *m_modelCursor;
    Model2DSlice *m_model2DSlice;
    Model2DTransformation *m_model2DTransformation;

    MainWindow *m_mainWindow;

    ModelROISetEvent newROI(roi_type type, roi_plane plane, int id);
    void update2DSlice(roi &roi);

    int new_id();
    size_t splitString(const std::string &txt, std::vector<std::string> &strs, char ch);
    bool replaceString(std::string& str, const std::string& from, const std::string& to);
    float area(std::vector<float> contour);
    bool insideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy,float Px, float Py);
    bool snip(std::vector<float> contour,int u,int v,int w,int n,int *V);
    bool triangulatePolygon(std::vector<float> contour, std::vector<float> result);

    bool m_isEnabled;
    StateManager * m_stateManager;
    int  m_roiStateChangedHandlerSlotID;

    bool m_roiEditionActivated;

    int m_currentEditableTractID;
};

#endif // MODELROI_H
