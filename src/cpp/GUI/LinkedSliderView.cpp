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
    fGain = registerVstParam(fParams->fLeftGainParam, false);
    fLinkedGain = registerVstParam(fParams->fRightGainParam);
  }
  else
  {
    fGain = registerVstParam(fParams->fRightGainParam, false);
    fLinkedGain = registerVstParam(fParams->fLeftGainParam);
  }
}

//------------------------------------------------------------------------
// LinkedSliderView::onParameterChange
//------------------------------------------------------------------------
void LinkedSliderView::onParameterChange(ParamID iParamID)
{
  if(fLinkedGain->getParamID() == iParamID)
  {
    if(fLink->getValue())
      fGain->setValue(fLinkedGain->getValue());
    return;
  }

  if(fLink->getParamID() == iParamID)
  {
    // case when we went from not linked -> linked
    if(fLink->getValue())
    {
      if(fGain->getValue().getValue() < fLinkedGain->getValue().getValue())
        fGain->setValue(fLinkedGain->getValue());
    }
  }
}

LinkedSliderView::Creator __gLinkedSliderCreator("JSGain::LinkedSlider", "JSGain - LinkedSlider");

}
}
}
}
