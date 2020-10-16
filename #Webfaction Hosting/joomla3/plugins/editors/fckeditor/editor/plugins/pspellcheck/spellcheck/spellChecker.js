/**
* @version		$Id: fckeditor.php 1154 18-1-2008 AW
* @package		JoomlaFCK
* @copyright	Copyright (C) 2006 - 2008 WebXSolution Ltd. All rights reserved.
* @license		Creative Commons Licence
* The code for this additional work for the FCKeditor has been  been written/modified by WebxSolution Ltd.
* You may not copy or distribute the work without written consent
* from WebxSolution Ltd.
*/
function spellChecker( textObject ){this.popUpUrl = 'spellchecker.html';	this.popUpName = 'spellchecker'; this.popUpProps = null; this.replWordFlag = "R";	this.ignrWordFlag = "I";	this.replAllFlag = "RA";	this.ignrAllFlag = "IA";	this.fromReplAll = "~RA";	this.fromIgnrAll = "~IA";		this.wordFlags = new Array(); this.currentTextIndex = 0; this.currentWordIndex = 0; this.spellCheckerWin = null; this.controlWin = null; this.wordWin = null; this.textArea = textObject;	this.textInputs = arguments; this._spellcheck = _spellcheck;	this._getSuggestions = _getSuggestions; this._setAsIgnored = _setAsIgnored; this._getTotalReplaced = _getTotalReplaced; this._setWordText = _setWordText; this._getFormInputs = _getFormInputs; this.openChecker = openChecker; this.startCheck = startCheck; this.checkTextBoxes = checkTextBoxes; this.checkTextAreas = checkTextAreas; this.spellCheckAll = spellCheckAll; this.ignoreWord = ignoreWord; this.ignoreAll = ignoreAll; this.replaceWord = replaceWord; this.replaceAll = replaceAll; this.terminateSpell = terminateSpell; this.undo = undo; this.move = move;window.speller = this;}; function checkTextBoxes(){this.textInputs = this._getFormInputs( "^text$" ); this.openChecker();}; function checkTextAreas(){this.textInputs = this._getFormInputs( "^textarea$" ); this.openChecker();}; function spellCheckAll(){this.textInputs = this._getFormInputs( "^text(area)?$" ); this.openChecker();}; function openChecker(){this.spellCheckerWin = window.open( this.popUpUrl,this.popUpName,this.popUpProps ); if( !this.spellCheckerWin.opener ){this.spellCheckerWin.opener = window;}}; function startCheck( wordWindowObj,controlWindowObj ){this.wordWin = wordWindowObj; this.controlWin = controlWindowObj; this.wordWin.resetForm(); this.controlWin.resetForm(); this.currentTextIndex = 0; this.currentWordIndex = 0; this.wordFlags = new Array( this.wordWin.textInputs.length ); for( var i=0;i<this.wordFlags.length;i++ ){this.wordFlags[i] = [];}; this._spellcheck(); return true;}; function ignoreWord(){var wi = this.currentWordIndex; var ti = this.currentTextIndex; if( !this.wordWin ){alert( 'Error: Word frame not available.' ); return false;}; if( !this.wordWin.getTextVal( ti,wi )){alert( 'Error: "Not in dictionary" text is missing.' ); return false;}; if( this._setAsIgnored( ti,wi,this.ignrWordFlag )){this.currentWordIndex++; this._spellcheck();}; return true;}; function ignoreAll(){var wi = this.currentWordIndex; var ti = this.currentTextIndex; if( !this.wordWin ){alert( 'Error: Word frame not available.' ); return false;}; var s_word_to_repl = this.wordWin.getTextVal( ti,wi ); if( !s_word_to_repl ){alert( 'Error: "Not in dictionary" text is missing' ); return false;}; this._setAsIgnored( ti,wi,this.ignrAllFlag ); for( var i = ti;i < this.wordWin.textInputs.length;i++ ){for( var j = 0;j < this.wordWin.totalWords( i );j++ ){if(( i == ti && j > wi ) || i > ti ){if(( this.wordWin.getTextVal( i,j ) == s_word_to_repl )&& ( !this.wordFlags[i][j] )){this._setAsIgnored( i,j,this.fromIgnrAll );}}}}; this.currentWordIndex++; this._spellcheck(); return true;}; function replaceWord(){var wi = this.currentWordIndex; var ti = this.currentTextIndex; if( !this.wordWin ){alert( 'Error: Word frame not available.' ); return false;}; if( !this.wordWin.getTextVal( ti,wi )){alert( 'Error: "Not in dictionary" text is missing' ); return false;}; if( !this.controlWin.replacementText ){return false ;}; var txt = this.controlWin.replacementText; if( txt.value ){var newspell = new String( txt.value ); if( this._setWordText( ti,wi,newspell,this.replWordFlag )){this.currentWordIndex++; this._spellcheck();}}; return true;}; function replaceAll(){var ti = this.currentTextIndex; var wi = this.currentWordIndex; if( !this.wordWin ){alert( 'Error: Word frame not available.' ); return false;}; var s_word_to_repl = this.wordWin.getTextVal( ti,wi ); if( !s_word_to_repl ){alert( 'Error: "Not in dictionary" text is missing' ); return false;}; var txt = this.controlWin.replacementText; if( !txt.value ) return false; var newspell = new String( txt.value ); this._setWordText( ti,wi,newspell,this.replAllFlag ); for( var i = ti;i < this.wordWin.textInputs.length;i++ ){for( var j = 0;j < this.wordWin.totalWords( i );j++ ){if(( i == ti && j > wi ) || i > ti ){if(( this.wordWin.getTextVal( i,j ) == s_word_to_repl )&& ( !this.wordFlags[i][j] )){this._setWordText( i,j,newspell,this.fromReplAll );}}}}; this.currentWordIndex++; this._spellcheck(); return true;}; function terminateSpell(){var msg = "";		var numrepl = this._getTotalReplaced(); if( numrepl == 0 ){if( !this.wordWin ){msg = "";}else{if( this.wordWin.totalMisspellings() ){msg += FCKLang.DlgSpellNoChanges ;}else{msg += FCKLang.DlgSpellNoMispell ;}}}else if( numrepl == 1 ){msg += FCKLang.DlgSpellOneChange ;}else{msg += FCKLang.DlgSpellManyChanges.replace( /%1/g,numrepl ) ;}; if( msg ){alert( msg );}; if( numrepl > 0 ){for( var i = 0;i < this.textInputs.length;i++ ){if( this.wordWin ){if( this.wordWin.textInputs[i] ){this.textInputs[i].value = this.wordWin.textInputs[i];}}}}; if ( typeof( this.OnFinished ) == 'function' )			this.OnFinished(numrepl); return true;}; function undo(){var ti = this.currentTextIndex; var wi = this.currentWordIndex; if( this.wordWin.totalPreviousWords( ti,wi ) > 0 ){this.wordWin.removeFocus( ti,wi ); do{if( this.currentWordIndex == 0 && this.currentTextIndex > 0 ){this.currentTextIndex--; this.currentWordIndex = this.wordWin.totalWords( this.currentTextIndex )-1; if( this.currentWordIndex < 0 ) this.currentWordIndex = 0;}else{if( this.currentWordIndex > 0 ){this.currentWordIndex--;}}}while ( this.wordWin.totalWords( this.currentTextIndex ) == 0|| this.wordFlags[this.currentTextIndex][this.currentWordIndex] == this.fromIgnrAll|| this.wordFlags[this.currentTextIndex][this.currentWordIndex] == this.fromReplAll); var text_idx = this.currentTextIndex; var idx = this.currentWordIndex; var preReplSpell = this.wordWin.originalSpellings[text_idx][idx]; if( this.wordWin.totalPreviousWords( text_idx,idx ) == 0 ){this.controlWin.disableUndo();}; var i,j,origSpell; switch( this.wordFlags[text_idx][idx] ){case this.replAllFlag : for( i = text_idx;i < this.wordWin.textInputs.length;i++ ){for( j = 0;j < this.wordWin.totalWords( i );j++ ){if(( i == text_idx && j >= idx ) || i > text_idx ){origSpell = this.wordWin.originalSpellings[i][j]; if( origSpell == preReplSpell ){this._setWordText ( i,j,origSpell,undefined );}}}}; break; case this.ignrAllFlag : for( i = text_idx;i < this.wordWin.textInputs.length;i++ ){for( j = 0;j < this.wordWin.totalWords( i );j++ ){if(( i == text_idx && j >= idx ) || i > text_idx ){origSpell = this.wordWin.originalSpellings[i][j]; if( origSpell == preReplSpell ){this.wordFlags[i][j] = undefined;}}}}; break; case this.replWordFlag : this._setWordText ( text_idx,idx,preReplSpell,undefined ); break;}; this.wordFlags[text_idx][idx] = undefined; this._spellcheck();}}; function move(ti,wi){if(this.currentTextIndex==ti && this.currentWordIndex == wi) return; this.wordWin.removeFocus( this.currentTextIndex,this.currentWordIndex); this.currentTextIndex = ti; this.currentWordIndex = wi; if(this.wordWin.totalPreviousWords(ti,wi)==0) this.controlWin.disableUndo(); var wf = this.wordFlags[ti][wi]; if(wf==this.replAllFlag || wf==this.fromReplAll ||wf==this.ignrAllFlag || wf==this.fromIgnrAll ){var preReplSpell = this.wordWin.originalSpellings[ti][wi]; var i,j,origSpell,keepLooking; for(keepLooking=true,i=ti;i<this.wordWin.textInputs.length && keepLooking;i++ ){for(j=(i==ti?wi+1:0);j<this.wordWin.totalWords( i ) && keepLooking;j++ ){origSpell = this.wordWin.originalSpellings[i][j]; if( origSpell == preReplSpell ){if(this.wordFlags[i][j]==this.fromIgnrAll){this.wordFlags[i][j]=this.ignrAllFlag;keepLooking = false;} else if(this.wordFlags[i][j]==this.fromReplAll){this.wordFlags[i][j]=this.replAllFlag;keepLooking = false;}}}}}; this._setWordText ( ti,wi,this.wordWin.originalSpellings[ti][wi],undefined ); this._spellcheck();}; function _spellcheck(){var ti = this.currentTextIndex; var wi = this.currentWordIndex; var wrapped=false; var foundNext=false; var keepLooking=true; do{if(wrapped){if(ti>this.currentTextIndex) keepLooking = false; else if(wi==this.wordWin.totalWords(ti)){ti++;wi=0;} else if(ti==this.currentTextIndex && wi==this.currentWordIndex) keepLooking = false; else if(!this.wordFlags[ti][wi]){keepLooking = false;foundNext = true;} else wi++;} else if(ti == this.wordWin.textInputs.length){ti=0;wi=0;wrapped = true;} else if(wi == this.wordWin.totalWords(ti)){ti++;wi=0;} else if(!this.wordFlags[ti][wi]){keepLooking = false;foundNext = true;} else wi++;} while(keepLooking); if(!foundNext){this.terminateSpell(); return;}; if(this.wordWin.totalPreviousWords(ti,wi)>0) this.controlWin.enableUndo(); this.controlWin.evaluatedText.value = this.wordWin.originalSpellings[ti][wi]; this.wordWin.setFocus(ti,wi); this._getSuggestions(ti,wi); this.currentTextIndex = ti; this.currentWordIndex = wi;}; function _getSuggestions( text_num,word_num ){this.controlWin.clearSuggestions(); var a_suggests = this.wordWin.suggestions[text_num][word_num]; if( a_suggests ){for( var ii = 0;ii < a_suggests.length;ii++ ){this.controlWin.addSuggestion( a_suggests[ii] );}}; this.controlWin.selectDefaultSuggestion();}; function _setAsIgnored( text_num,word_num,flag ){this.wordWin.removeFocus( text_num,word_num ); this.wordFlags[text_num][word_num] = flag; return true;}; function _getTotalReplaced(){var i_replaced = 0; for( var i = 0;i < this.wordFlags.length;i++ ){for( var j = 0;j < this.wordFlags[i].length;j++ ){if(( this.wordFlags[i][j] == this.replWordFlag )|| ( this.wordFlags[i][j] == this.replAllFlag )|| ( this.wordFlags[i][j] == this.fromReplAll )){i_replaced++;}}}; return i_replaced;}; function _setWordText( text_num,word_num,newText,flag ){this.wordWin.setText( text_num,word_num,newText ); this.wordFlags[text_num][word_num] = flag; return true;}; function _getFormInputs( inputPattern ){var inputs = new Array(); for( var i = 0;i < document.forms.length;i++ ){for( var j = 0;j < document.forms[i].elements.length;j++ ){if( document.forms[i].elements[j].type.match( inputPattern )){inputs[inputs.length] = document.forms[i].elements[j];}}}; return inputs;};