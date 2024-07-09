#ifndef TOOL_TRACTOGRAPHY_H
#define TOOL_TRACTOGRAPHY_H

#include "core/Tool.h"
#include "../controllers/ControllerDTITractographyToolTab.h"
#include <modules/visualization/submodules/roi/controllers/ControllerROIToolTab.h>

class ToolTractography: public Tool<ControllerDTITractographyToolTab, ControllerROIToolTab>
{
public:
    ToolTractography();
    ~ToolTractography() { }

    void updateController(ControllerDTITractographyToolTab *controller);
    void updateController(ControllerROIToolTab *controller);
    void updateComponents();

    virtual void updateTractography() = 0;
    virtual void updateTractographyEnabled() = 0;

protected:
    ControllerDTITractographyToolTab* getControllerTractography() const;
    ControllerROIToolTab* getControllerROI() const;

private:
    ControllerDTITractographyToolTab *m_ptrControllerTractography;
    ControllerROIToolTab *m_controllerROIToolTab;
};

#endif // TOOL_Tractography_H
