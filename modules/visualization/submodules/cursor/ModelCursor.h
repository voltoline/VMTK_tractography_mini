#ifndef MODEL_CURSOR_H
#define MODEL_CURSOR_H

#include <QMap>
#include "model/ModelSubject.h"
#include "model/EventNotifier.h"
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/Model2DSlice.h"
#else
#include "modules/visualization/submodules/2d_slice/model/Model2DSlice.h"
#endif

#include "modules/visualization/StateManager.h"

enum class ModelCursorEvent
{
    NONE,
    CURSOR_STATE_CHANGED,
    CURSOR_MODE_CHANGED,
    CURSOR_POSITION_CHANGED,
    CURSOR_DRAW_TYPE_CHANGED,
    CURSOR_DEPTH_BUFFER_CHANGED,
    CURSOR_OFFSCREEN_CHANGED
};

enum DrawCursorType{
    crossCursor,
    radiusCursor,
    lineCursor,
    invisibleCursor
};

class ModelCursor:
        public EventNotifier<ModelCursorEvent, ModelCursor*, ModelCursorEvent>,
        public ModelObserver<Model2DSlice, Model2DSliceEvent>
{

public:

    enum class Mode
    {
        MOVE,
        LOCK
    };

    ModelCursor();
    ~ModelCursor();

    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

    ModelCursorEvent setCursorPositionFrom3DView(glm::ivec2 mousePos, glm::vec4 viewport, glm::ivec2 screen_size, float depthZ);
    ModelCursorEvent setCursorPositionFromAxialView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2]);
    ModelCursorEvent setCursorPositionFromCoronalView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2]);
    ModelCursorEvent setCursorPositionFromSagittalView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2]);

    void getCursorPosition3DAsSliceCoordinates(unsigned short vec[]);
    glm::vec3 getCursorPosition3D();
    glm::vec3 getCursorPositionAxial();
    glm::vec3 getCursorPositionCoronal();
    glm::vec3 getCursorPositionSagittal();

    void setRadiologicalVoxelOrder();
    void setNeurologicalVoxelOrder();

    void setPhyScale(const float phy_scale[3]);
    void setScaleFactors(const float scale_factors[4]);
    void setFlagSync2DSlice(bool flagSync2DSlice);

    void setModel2DSlice(Model2DSlice *model2DSlice);
    void setStateManager(StateManager *state_manager);

    ModelCursorEvent setActivated(bool state);
    bool isActivated() const;

    bool isLocked() const;
    bool isMovable() const;

    void handleCursorStateChanged(std::string feature_name, FeatureState state);

    Mode getCursorMode() const;
    ModelCursorEvent setCursorMode(const Mode &cursorMode);

    ModelCursorEvent setCursorPosition3D(glm::vec3 cursor_position);

    void connectModel3DTransformation();

    bool isEnabled() const;

    void setModelViewMatrix3D(const glm::mat4 &modelViewMatrix3D);
    void setProjectionMatrix3D(const glm::mat4 &projectionMatrix3D);

    void setModelViewMatrixAxial(const glm::mat4 &modelViewMatrixAxial);
    void setProjectionMatrixAxial(const glm::mat4 &projectionMatrixAxial);

    void setModelViewMatrixCoronal(const glm::mat4 &modelViewMatrixCoronal);
    void setProjectionMatrixCoronal(const glm::mat4 &projectionMatrixCoronal);

    void setModelViewMatrixSagittal(const glm::mat4 &modelViewMatrixSagittal);
    void setProjectionMatrixSagittal(const glm::mat4 &projectionMatrixSagittal);

    DrawCursorType getDrawCursorType() const;
    ModelCursorEvent setDrawCursorType(DrawCursorType newDrawCursorType);

    QMap<DrawCursorType, QString> getMapDrawCursorType() const;

    ModelCursorEvent setOffScreenEnable(QString name, bool enable);

    const QPair<QString, bool> &getOffScreenEnablePair() const;

private:
    void update2DSlices();

    enum class VoxelOrder{
        RAS,
        LAS
    };

    typedef struct _slice_data
    {
        unsigned short m_axial;
        unsigned short m_coronal;
        unsigned short m_sagittal;

    } SliceData;

    float m_cursorPosition3D[3];
    VoxelOrder m_voxelOrder;

    Model2DSlice *m_model2DSlice;
    StateManager *m_stateManager;

    int m_movableCursorStateChangedHandlerSlotID;
    int m_lockedCursorStateChangedHandlerSlotID;

    float m_phyScale[3];
    float m_scaleFactors[4];

    int m_mousePosX;
    int m_mousePosY;

    bool m_flagSync2DSlice;
    bool m_isActivated;
    bool m_isEnabled;

    DrawCursorType m_drawCursorType;

    QMap<DrawCursorType,QString> mapDrawCursorType;

    SliceData m_sliceData;

    Mode m_cursorMode;

    glm::mat4 m_modelViewMatrix3D;
    glm::mat4 m_projectionMatrix3D;

    glm::mat4 m_modelViewMatrixAxial;
    glm::mat4 m_projectionMatrixAxial;

    glm::mat4 m_modelViewMatrixCoronal;
    glm::mat4 m_projectionMatrixCoronal;

    glm::mat4 m_modelViewMatrixSagittal;
    glm::mat4 m_projectionMatrixSagittal;

    QPair<QString,bool> m_OffScreenEnablePair;

};
#endif // MODEL_CURSOR_H
