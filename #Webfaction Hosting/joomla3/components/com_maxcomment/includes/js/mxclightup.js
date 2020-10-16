function mxclightup(imageobject, opacity){
	 if ((navigator.appName.indexOf("Netscape")!=-1) && (parseInt(navigator.appVersion)>=5)){
		imageobject.style.MozOpacity=opacity/100;
	 } else if ((navigator.appName.indexOf("Microsoft")!= -1) && (parseInt(navigator.appVersion)>=4)){
		imageobject.filters.alpha.opacity=opacity;
	 }
}		
