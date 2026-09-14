class Components.Meter
{
	var Mask;
	var Bar;
   var MeterContainer;
	var FadeOut;
	var StartFadeOutTime;
	var FadeInterval;
	var targetPercent;
	var oldPercent;
	var meterTime;
	var meterDuration;
	var MeterContainerBarColor;
	var MeterContainerBorderColor;
	var MaskWidth;
	var idleTimer;
   function Meter(aContainer, aInvert)
   {
	  this.MeterContainer = aContainer;
	  this.Bar = aContainer.ChargeMeter_mc;
      this.MeterContainerBarColor = new Color(this.Bar.Bar);
      this.MeterContainerBorderColor = new Color(aContainer.Border.Color);
      this.MeterContainerBGColor = new Color(aContainer.Border.BG);
	   
	   this.MaskWidth = this.Bar.Mask._width
		this.maxWidth = this.Bar.Mask._x;
		this.minWidth = this.Bar.Mask._x - this.MaskWidth;
		this.Percent = (this.maxWidth - this.minWidth) / 100;
	  
      this.targetPercent = 0;
      this.oldPercent = 0;
      this.meterDuration = 200;
		this.meterTime = 0;
		this.idleTimer = 10000;
	  
	  
		this.FadeOut = true;
		this.StartFadeOutTime = 0;
		this.FadeInterval = 1000;
		this.MeterContainer._alpha = 0;
   }
   function setPosition(xpos, ypos, rot, xscale, yscale, abarColor, aborderColor, abgColor, aFadeOut, aCurrentTime)
   {
		this.MeterContainer._xscale = xscale;
		this.MeterContainer._yscale = yscale;
		this.MeterContainer._x = xpos;
		this.MeterContainer._y = ypos;
		this.MeterContainer._rotation = rot;
		this.MeterContainerBarColor.setRGB(abarColor);
		this.MeterContainerBorderColor.setRGB(aborderColor);
		this.MeterContainerBGColor.setRGB(abgColor);
		
		//if (aCurrentTime > this.meterTime + this.idleTimer)
		//{
		//	aFadeOut = true;
		//}
		
		if(this.FadeOut != aFadeOut)
		{
			this.StartFadeOutTime = aCurrentTime
			this.FadeOut = aFadeOut;
		}
		
		var alphaMin = 0;
		var alphaMax = 100;
		if (this.FadeOut == true)
		{
			alphaMin = 100;
			alphaMax = 0;
		}
		this.MeterContainer._alpha = Math.floor(this.Lerp(alphaMin, alphaMax, this.StartFadeOutTime, this.StartFadeOutTime + this.FadeInterval, aCurrentTime, !this.FadeOut));
		
		
   }
   
   function setMeterPercent(CurrentPercent, aCurrentTime, barGrowth)
   {
      CurrentPercent = this.doValueClamp(CurrentPercent);
		if(this.targetPercent != CurrentPercent)
		{
			this.oldPercent = this.targetPercent;
			this.targetPercent = CurrentPercent;
			this.meterTime = aCurrentTime;
		}
		var _loc1_ = false;
		if (this.oldPercent < this.targetPercent)
		{
			_loc1_ = true;		
		}
	  
	  var minB = this.minWidth;
	  var value = this.Percent * Math.floor(this.Lerp(this.oldPercent, this.targetPercent, this.meterTime, this.meterTime + this.meterDuration, aCurrentTime, _loc1_))
	  if (barGrowth)
	  {
		minB = this.maxWidth + (this.MaskWidth * 1);
		value = value - (value * 2)
	  }

	  this.Bar.Mask._x = minB + value
   }
   
   function doValueClamp(clampValue)
   {
      return clampValue <= 100 ? (clampValue > 0 ? clampValue : -1) : 100;
   }
   
   function Lerp(aTargetMin, aTargetMax, aSourceMin, aSourceMax, aSource, abClamp)
   {
      var _loc7_ = aTargetMin + (aSource - aSourceMin) / (aSourceMax - aSourceMin) * (aTargetMax - aTargetMin);
      if(abClamp)
      {
         _loc7_ = Math.min(Math.max(_loc7_,aTargetMin),aTargetMax);
      }
	  else
	  {
         _loc7_ = Math.max(Math.min(_loc7_,aTargetMin),aTargetMax);
	  }
      return _loc7_;
   }
}
