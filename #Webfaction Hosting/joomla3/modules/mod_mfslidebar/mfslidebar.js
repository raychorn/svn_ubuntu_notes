/**
* mfslidebar.js
* (c) Copyright Marcofolio.net
*
* For more great Joomla! extensions, please check http://www.marcofolio.net/
*/

var isExtended = 0;

function mfslideSlideBar(){

	new Effect.toggle('mfslideBarContents', 'blind', {scaleX: 'true', scaleY: 'true;', scaleContent: false});
	
	if(isExtended==0){
		$('mfslideBarTab').childNodes[0].src = $('mfslideBarTab').childNodes[0].src.replace(/(\.[^.]+)$/, '-active$1');
		
		new Effect.Fade('mfslideBarContents',
   	{ duration:1.0, from:0.0, to:1.0 });
		
		isExtended++;
	}
	else{
		$('mfslideBarTab').childNodes[0].src = $('mfslideBarTab').childNodes[0].src.replace(/-active(\.[^.]+)$/, '$1');
		
		new Effect.Fade('mfslideBarContents',
   	{ duration:1.0, from:1.0, to:0.0 });
		
		isExtended=0;
	}
	
}

function init(){
	Event.observe('mfslideBarTab', 'click', mfslideSlideBar, true);
}

Event.observe(window, 'load', init, true);