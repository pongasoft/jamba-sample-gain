#include "LinkedSliderView.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

//------------------------------------------------------------------------
// LinkedSliderView::setTag
//------------------------------------------------------------------------
void LinkedSliderView::setTag(int32_t val)
{
  CSlider::setTag(val);
  registerParameters();
}

//------------------------------------------------------------------------
// LinkedSliderView::registerParameters
//------------------------------------------------------------------------
void LinkedSliderView::registerParameters()
{
  if(!fParamCxMgr || getTag() < 0)
    return;

  fLink = registerVstParam(fParams->fLinkParam);
  if(getTag() == fParams->fLeftGainParam->fParamID)
  {
    fGain = registerRawVstParam(fParams->fLeftGainParam->fParamID, false);
    fLinkedGain = registerRawVstParam(fParams->fRightGainParam->fParamID);

  }
  else
  {
    fGain = registerRawVstParam(fParams->fRightGainParam->fParamID, false);
    fLinkedGain = registerRawVstParam(fParams->fLeftGainParam->fParamID);
  }
}

//------------------------------------------------------------------------
// LinkedSliderView::onParameterChange
//------------------------------------------------------------------------
void LinkedSliderView::onParameterChange(ParamID iParamID)
{
  if(fLinkedGain.getParamID() == iParamID)
  {
    if(fLink)
    {
      if(fGain != fLinkedGain)
        fGain.setValue(fLinkedGain);
    }
    return;
  }

  if(fLink.getParamID() == iParamID)
  {
    // case when we went from not linked -> linked
    if(fLink)
    {
      if(fGain.getValue() < fLinkedGain.getValue())
      {
        fGain.setValue(fLinkedGain);
      }
    }
  }
}

LinkedSliderView::Creator __gLinkedSliderCreator("JSGain::LinkedSlider", "JSGain - LinkedSlider");

}
}
}
}
