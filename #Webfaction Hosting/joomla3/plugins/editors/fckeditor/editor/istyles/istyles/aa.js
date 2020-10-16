/**
* @version		$Id: fckeditor.php 1154 18-1-2008 AW
* @package		JoomlaFCK
* @copyright	Copyright (C) 2006 - 2008 WebXSolution Ltd. All rights reserved.
* @license		Creative Commons Licence
* The code for this additional work for the FCKeditor has been  been written/modified by WebxSolution Ltd.
* You may not copy or distribute the work without written consent
* from WebxSolution Ltd.
*/
var oEditor = window.parent.InnerDialogLoaded(); var defaultFonts = "" +"Arial,Helvetica,sans-serif=Arial,Helvetica,sans-serif;" +"Times New Roman,Times,serif=Times New Roman,Times,serif;" +"Courier New,Courier,mono=Courier New,Courier,mono;" +"Times New Roman,Times,serif=Times New Roman,Times,serif;" +"Georgia,Times New Roman,Times,serif=Georgia,Times New Roman,Times,serif;" +"Verdana,Arial,Helvetica,sans-serif=Verdana,Arial,Helvetica,sans-serif;" +"Geneva,Arial,Helvetica,sans-serif=Geneva,Arial,Helvetica,sans-serif"; var defaultSizes = "9;10;12;14;16;18;24;xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger"; var defaultMeasurement = "+pixels=px;points=pt;in;cm;mm;picas;ems;exs;%"; var defaultSpacingMeasurement = "pixels=px;points=pt;in;cm;mm;picas;+ems;exs;%"; var defaultIndentMeasurement = "pixels=px;+points=pt;in;cm;mm;picas;ems;exs;%"; var defaultWeight = "normal;bold;bolder;lighter;100;200;300;400;500;600;700;800;900"; var defaultTextStyle = "normal;italic;oblique"; var defaultVariant = "normal;small-caps"; var defaultLineHeight = "normal"; var defaultAttachment = "fixed;scroll"; var defaultRepeat = "no-repeat;repeat;repeat-x;repeat-y"; var defaultPosH = "left;center;right"; var defaultPosV = "top;center;bottom"; var defaultVAlign = "baseline;sub;super;top;text-top;middle;bottom;text-bottom"; var defaultDisplay = "inline;block;list-item;run-in;compact;marker;table;inline-table;table-row-group;table-header-group;table-footer-group;table-row;table-column-group;table-column;table-cell;table-caption;none"; var defaultBorderStyle = "none;solid;dashed;dotted;double;groove;ridge;inset;outset"; var defaultBorderWidth = "thin;medium;thick"; var defaultListType = "disc;circle;square;decimal;lower-roman;upper-roman;lower-alpha;upper-alpha;none"; function Ok(){var css=generateCSS(); if ( typeof(window.parent.Args().CustomValue) == 'function' )window.parent.Args().CustomValue( css ); return true;}; function OnDialogTabChange( tabCode ){ShowE('text_panel',( tabCode == 'text_tab' ) );ShowE('background_panel',( tabCode == 'background_tab' ) );ShowE('block_panel',( tabCode == 'block_tab' ) );ShowE('box_panel', (tabCode == 'box_tab') );ShowE('border_panel',(tabCode == 'border_tab'));ShowE('list_panel',(tabCode == 'list_tab') );ShowE('positioning_panel', (tabCode == 'positioning_tab') ) ;}; function init(){oEditor.FCKLanguageManager.TranslatePage(document);OnDialogTabChange('text_tab');window.parent.SetOkButton( true );window.parent.AddTab( 'text_tab','Text' );window.parent.AddTab( 'background_tab','Background' );window.parent.AddTab( 'block_tab','Block' );window.parent.AddTab( 'box_tab','Box' );window.parent.AddTab( 'border_tab','Border' );window.parent.AddTab( 'list_tab','List' );window.parent.AddTab( 'positioning_tab','Positioning' ); var defaultFonts = "" +"Arial;" +"'Times New Roman';" +"'Courier New';" +"'Times New Roman';" +"Georgia,'Times New Roman';" +"Verdana;" +"Geneva"; var defaultSizes = "9;10;12;14;16;18;24;xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger"; var defaultMeasurement = "px;pt;in;cm;mm;picas;ems;exs;%"; var defaultSpacingMeasurement = "px;pt;in;cm;mm;picas;+ems;exs;%"; var defaultIndentMeasurement = "px;pt;in;cm;mm;picas;ems;exs;%"; var defaultWeight = "normal;bold;bolder;lighter;100;200;300;400;500;600;700;800;900"; var defaultTextStyle = "normal;italic;oblique"; var defaultVariant = "normal;small-caps"; var defaultLineHeight = "normal";	var defaultAttachment = "fixed;scroll"; var defaultRepeat = "no-repeat;repeat;repeat-x;repeat-y"; var defaultPosH = "left;center;right";	var defaultPosV = "top;center;bottom";	var defaultVAlign = "baseline;sub;super;top;text-top;middle;bottom;text-bottom"; var defaultDisplay = "inline;block;list-item;run-in;compact;marker;table;inline-table;table-row-group;table-header-group;table-footer-group;table-row;table-column-group;table-column;table-cell;table-caption;none"; var defaultBorderStyle = "none;solid;dashed;dotted;double;groove;ridge;inset;outset"; var defaultBorderWidth = "thin;medium;thick"; var defaultListType = "disc;circle;square;decimal;lower-roman;upper-roman;lower-alpha;upper-alpha;none"; var defaultSpacing = "normal"; var textCase="capitalize;uppercase;lowercase";fillCombo('text_font','style_font',defaultFonts,';',true);fillCombo('text_size','style_font_size',defaultSizes,';',true);fillCombo('text_size_measurement','style_font_size_measurement',defaultMeasurement,';',true);fillCombo('text_case','style_text_case',"capitalize;uppercase;lowercase",';',true);fillCombo('text_weight','style_font_weight',defaultWeight,';',true);fillCombo('text_style','style_font_style',defaultTextStyle,';',true);fillCombo('text_variant','style_font_variant',defaultVariant,';',true);fillCombo('text_lineheight_opt','style_font_line_height_opt',defaultLineHeight,';',true);fillCombo('text_lineheight_measurement','style_font_line_height_measurement',defaultMeasurement,';',true);fillCombo('background_attachment','style_background_attachment',defaultAttachment,';',true);fillCombo('background_repeat','style_background_repeat',defaultRepeat,';',true);fillCombo('background_hpos_opt','style_background_hpos_opt',defaultPosH,';',true);fillCombo('background_hpos_measurement','style_background_hpos_measurement',defaultMeasurement,';',true);fillCombo('background_vpos_opt','style_background_vpos_opt',defaultPosV,';',true);fillCombo('background_vpos_measurement','style_background_vpos_measurement',defaultMeasurement,';',true);fillCombo('block_wordspacing_opt','style_wordspacing_measurement',defaultSpacing,';',true);fillCombo('block_wordspacing_measurement','style_wordspacing_measurement',defaultSpacingMeasurement,';',true);fillCombo('block_letterspacing_opt','style_letterspacing_measurement',defaultSpacing,';',true);fillCombo('block_letterspacing_measurement','style_letterspacing_measurement',defaultSpacingMeasurement,';',true);fillCombo('block_vertical_alignment','style_vertical_alignment',defaultVAlign,';',true);fillCombo('block_text_align','style_text_align',"left;right;center;justify",';',true);fillCombo('block_whitespace','style_whitespace',"normal;pre;nowrap",';',true);fillCombo('block_display','style_display',defaultDisplay,';',true);fillCombo('block_text_indent_measurement','style_text_indent_measurement',defaultIndentMeasurement,';',true);fillCombo('box_width_measurement','style_box_width_measurement',defaultMeasurement,';',true);fillCombo('box_height_measurement','style_box_height_measurement',defaultMeasurement,';',true);fillCombo('box_float','style_float','left;right;none',';',true);fillCombo('box_clear','style_clear','left;right;both;none',';',true);fillCombo('box_padding_left_measurement','style_padding_left_measurement',defaultMeasurement,';',true);fillCombo('box_padding_top_measurement','style_padding_top_measurement',defaultMeasurement,';',true);fillCombo('box_padding_bottom_measurement','style_padding_bottom_measurement',defaultMeasurement,';',true);fillCombo('box_padding_right_measurement','style_padding_right_measurement',defaultMeasurement,';',true);fillCombo('box_margin_left_measurement','style_margin_left_measurement',defaultMeasurement,';',true);fillCombo('box_margin_top_measurement','style_margin_top_measurement',defaultMeasurement,';',true);fillCombo('box_margin_bottom_measurement','style_margin_bottom_measurement',defaultMeasurement,';',true);fillCombo('box_margin_right_measurement','style_margin_right_measurement',defaultMeasurement,';',true);fillCombo('border_style_top','style_border_style_top',defaultBorderStyle,';',true);fillCombo('border_style_right','style_border_style_right',defaultBorderStyle,';',true);fillCombo('border_style_bottom','style_border_style_bottom',defaultBorderStyle,';',true);fillCombo('border_style_left','style_border_style_left',defaultBorderStyle,';',true);fillCombo('border_width_top_opt','style_border_width_top',defaultBorderWidth,';',true);fillCombo('border_width_right_opt','style_border_width_right',defaultBorderWidth,';',true);fillCombo('border_width_bottom_opt','style_border_width_bottom',defaultBorderWidth,';',true);fillCombo('border_width_left_opt','style_border_width_left',defaultBorderWidth,';',true);fillCombo('border_width_top_measurement','style_border_width_top_measurement',defaultMeasurement,';',true);fillCombo('border_width_right_measurement','style_border_width_right_measurement',defaultMeasurement,';',true);fillCombo('border_width_bottom_measurement','style_border_width_bottom_measurement',defaultMeasurement,';',true);fillCombo('border_width_left_measurement','style_border_width_left_measurement',defaultMeasurement,';',true);fillCombo('list_type','style_list_type',defaultListType,';',true);fillCombo('list_position','style_list_position',"inside;outside",';',true);fillCombo('positioning_type','style_positioning_type',"relative;static;absolute",';',true);fillCombo('positioning_visibility','style_positioning_visibility',"inherit;visible;hidden",';',true);fillCombo('positioning_width_measurement','style_positioning_width_measurement',defaultMeasurement,';',true);fillCombo('positioning_height_measurement','style_positioning_height_measurement',defaultMeasurement,';',true);fillCombo('positioning_overflow','style_positioning_overflow',"visible;hidden;scroll;auto",';',true);fillCombo('positioning_placement_top_measurement','style_positioning_placement_top_measurement',defaultMeasurement,';',true);fillCombo('positioning_placement_right_measurement','style_positioning_placement_right_measurement',defaultMeasurement,';',true);fillCombo('positioning_placement_bottom_measurement','style_positioning_placement_bottom_measurement',defaultMeasurement,';',true);fillCombo('positioning_placement_left_measurement','style_positioning_placement_left_measurement',defaultMeasurement,';',true);fillCombo('positioning_clip_top_measurement','style_positioning_clip_top_measurement',defaultMeasurement,';',true);fillCombo('positioning_clip_right_measurement','style_positioning_clip_right_measurement',defaultMeasurement,';',true);fillCombo('positioning_clip_bottom_measurement','style_positioning_clip_bottom_measurement',defaultMeasurement,';',true);fillCombo('positioning_clip_left_measurement','style_positioning_clip_left_measurement',defaultMeasurement,';',true);setupFormData();}; function fillCombo(id,bulk,string,tokenizer){var elem=document.getElementById(id); var out=""; var s=string.split(tokenizer);elem.options[0]=new Option('',''); for(i=0;i<s.length;i++){var text = s[i].replace(/'/g,'');elem.options[i+1]=new Option(text,s[i]);elem.options[0].selected=true;}}; function selectByValue(f,id,cssvalue,f1,f2){document.getElementById(id).value= cssvalue;}; function setupFormData(){var ce = document.getElementById('container'),f = document.forms[0],s,b,i; var ref=window.location.href;ref=ref.substring(ref.indexOf('#')+1,ref.length);ref=ref.replace(new RegExp('%20','gi'),' ');ce.style.cssText=ref;selectByValue(f,'text_font',UCFirstLetter(ce.style.fontFamily),true,true);selectByValue(f,'text_size',getNum(ce.style.fontSize),true,true);selectByValue(f,'text_size_measurement',getMeasurement(ce.style.fontSize));selectByValue(f,'text_weight',ce.style.fontWeight,true,true);selectByValue(f,'text_style',ce.style.fontStyle,true,true);selectByValue(f,'text_lineheight',getNum(ce.style.lineHeight),true,true);selectByValue(f,'text_lineheight_measurement',getMeasurement(ce.style.lineHeight));selectByValue(f,'text_case',ce.style.textTransform,true,true);selectByValue(f,'text_variant',ce.style.fontVariant,true,true); document.getElementById('text_color').style.background=ce.style.color;f.text_underline.checked = inStr(ce.style.textDecoration,'underline');f.text_overline.checked = inStr(ce.style.textDecoration,'overline');f.text_linethrough.checked = inStr(ce.style.textDecoration,'line-through');f.text_blink.checked = inStr(ce.style.textDecoration,'blink'); document.getElementById('background_color').style.background=ce.style.backgroundColor;f.background_image.value = ce.style.backgroundImage.replace(new RegExp("url\\('?([^']*)'?\\)",'gi'),"$1");selectByValue(f,'background_repeat',ce.style.backgroundRepeat,true,true);selectByValue(f,'background_attachment',ce.style.backgroundAttachment,true,true);selectByValue(f,'background_hpos',getNum(getVal(ce.style.backgroundPosition,0)),true,true);selectByValue(f,'background_hpos_measurement',getMeasurement(getVal(ce.style.backgroundPosition,0)));selectByValue(f,'background_vpos',getNum(getVal(ce.style.backgroundPosition,1)),true,true);selectByValue(f,'background_vpos_measurement',getMeasurement(getVal(ce.style.backgroundPosition,1)));selectByValue(f,'block_wordspacing',getNum(ce.style.wordSpacing),true,true);selectByValue(f,'block_wordspacing_measurement',getMeasurement(ce.style.wordSpacing));selectByValue(f,'block_letterspacing',getNum(ce.style.letterSpacing),true,true);selectByValue(f,'block_letterspacing_measurement',getMeasurement(ce.style.letterSpacing));selectByValue(f,'block_vertical_alignment',ce.style.verticalAlign,true,true);selectByValue(f,'block_text_align',ce.style.textAlign,true,true);f.block_text_indent.value = getNum(ce.style.textIndent);selectByValue(f,'block_text_indent_measurement',getMeasurement(ce.style.textIndent));selectByValue(f,'block_whitespace',ce.style.whiteSpace,true,true);selectByValue(f,'block_display',ce.style.display,true,true);f.box_width.value = getNum(ce.style.width);selectByValue(f,'box_width_measurement',getMeasurement(ce.style.width));f.box_height.value = getNum(ce.style.height);selectByValue(f,'box_height_measurement',getMeasurement(ce.style.height)); if (!document.all)selectByValue(f,'box_float',ce.style.cssFloat,true,true); else selectByValue(f,'box_float',ce.style.styleFloat,true,true);selectByValue(f,'box_clear',ce.style.clear,true,true);setupBox(f,ce,'box_padding','padding','');setupBox(f,ce,'box_margin','margin','');setupBox(f,ce,'border_style','border','Style');setupBox(f,ce,'border_width','border','Width');setupBox(f,ce,'border_color','border','Color'); document.getElementById('border_color_top').style.background=ce.style.borderTopColor; document.getElementById('border_color_right').style.background=ce.style.borderRightColor; document.getElementById('border_color_bottom').style.background=ce.style.borderBottomColor; document.getElementById('border_color_left').style.background=ce.style.borderLeftColor;selectByValue(f,'list_type',ce.style.listStyleType,true,true);selectByValue(f,'list_position',ce.style.listStylePosition,true,true);f.list_bullet_image.value = ce.style.listStyleImage.replace(new RegExp("url\\('?([^']*)'?\\)",'gi'),"$1");selectByValue(f,'positioning_type',ce.style.position,true,true);selectByValue(f,'positioning_visibility',ce.style.visibility,true,true);selectByValue(f,'positioning_overflow',ce.style.overflow,true,true);f.positioning_zindex.value = ce.style.zIndex ? ce.style.zIndex : "";f.positioning_width.value = getNum(ce.style.width);selectByValue(f,'positioning_width_measurement',getMeasurement(ce.style.width));f.positioning_height.value = getNum(ce.style.height);selectByValue(f,'positioning_height_measurement',getMeasurement(ce.style.height));setupBox(f,ce,'positioning_placement','','',new Array('top','right','bottom','left'));s = ce.style.clip.replace(new RegExp("rect\\('?([^']*)'?\\)",'gi'),"$1");s = s.replace(/,/g,' '); if (!hasEqualValues(new Array(getVal(s,0),getVal(s,1),getVal(s,2),getVal(s,3)))){f.positioning_clip_top.value = getNum(getVal(s,0));selectByValue(f,'positioning_clip_top_measurement',getMeasurement(getVal(s,0)));f.positioning_clip_right.value = getNum(getVal(s,1));selectByValue(f,'positioning_clip_right_measurement',getMeasurement(getVal(s,1)));f.positioning_clip_bottom.value = getNum(getVal(s,2));selectByValue(f,'positioning_clip_bottom_measurement',getMeasurement(getVal(s,2)));f.positioning_clip_left.value = getNum(getVal(s,3));selectByValue(f,'positioning_clip_left_measurement',getMeasurement(getVal(s,3)));}else{f.positioning_clip_top.value = getNum(getVal(s,0));selectByValue(f,'positioning_clip_top_measurement',getMeasurement(getVal(s,0)));f.positioning_clip_right.value = f.positioning_clip_bottom.value = f.positioning_clip_left.value;};_onload();}; function getMeasurement(s){return s.replace(/^([0-9]+)(.*)$/,"$2");}; function getNum(s){if (new RegExp('^[0-9]+[a-z%]+$','gi').test(s)) return s.replace(/[^0-9]/g,''); return s;}; function inStr(s,n){return new RegExp(n,'gi').test(s);}; function getVal(s,i){var a = s.split(" "); if (a.length > 1) return a[i]; return "";}; function setValue(f,n,v){if (document.getElementById(n).type == "text"){document.getElementById(n).value = v;} else selectByValue(f,n,v,true,true);}; function setupBox(f,ce,fp,pr,sf,b){if (typeof(b) == "undefined")b = new Array('Top','Right','Bottom','Left'); if (isSame(ce,pr,sf,b)){f.elements[fp + "_same"].checked = true;setValue(f,fp + "_top",getNum(ce.style[pr + b[0] + sf])); document.getElementById(fp + "_top").disabled = false; document.getElementById(fp + "_right").value = ""; document.getElementById(fp + "_right").disabled = true; document.getElementById(fp + "_bottom").value = ""; document.getElementById(fp + "_bottom").disabled = true; document.getElementById(fp + "_left").value = ""; document.getElementById(fp + "_left").disabled = true; if (f.elements[fp + "_top_measurement"]){selectByValue(f,fp + '_top_measurement',getMeasurement(ce.style[pr + b[0] + sf]));f.elements[fp + "_left_measurement"].disabled = true;f.elements[fp + "_bottom_measurement"].disabled = true;f.elements[fp + "_right_measurement"].disabled = true;}}else{f.elements[fp + "_same"].checked = false;setValue(f,fp + "_top",getNum(ce.style[pr + b[0] + sf])); document.getElementById(fp + "_top").disabled = false;setValue(f,fp + "_right",getNum(ce.style[pr + b[1] + sf])); document.getElementById(fp + "_right").disabled = false;setValue(f,fp + "_bottom",getNum(ce.style[pr + b[2] + sf])); document.getElementById(fp + "_bottom").disabled = false;setValue(f,fp + "_left",getNum(ce.style[pr + b[3] + sf])); document.getElementById(fp + "_left").disabled = false; if (f.elements[fp + "_top_measurement"]){selectByValue(f,fp + '_top_measurement',getMeasurement(ce.style[pr + b[0] + sf]));selectByValue(f,fp + '_right_measurement',getMeasurement(ce.style[pr + b[1] + sf]));selectByValue(f,fp + '_bottom_measurement',getMeasurement(ce.style[pr + b[2] + sf]));selectByValue(f,fp + '_left_measurement',getMeasurement(ce.style[pr + b[3] + sf]));f.elements[fp + "_left_measurement"].disabled = false;f.elements[fp + "_bottom_measurement"].disabled = false;f.elements[fp + "_right_measurement"].disabled = false;}}}; function isSame(e,pr,sf,b){var a = new Array(),i,x; if (typeof(b) == "undefined")b = new Array('Top','Right','Bottom','Left'); if (typeof(sf) == "undefined" || sf == null)sf = "";a[0] = e.style[pr + b[0] + sf];a[1] = e.style[pr + b[1] + sf];a[2] = e.style[pr + b[2] + sf];a[3] = e.style[pr + b[3] + sf]; for (i=0;i<a.length;i++){if (a[i] == null) return false; for (x=0;x<a.length;x++){if (a[x] != a[i]) return false;}}; return true;}; function hasEqualValues(a){var i,x; for (i=0;i<a.length;i++){if (a[i] == null) return false; for (x=0;x<a.length;x++){if (a[x] != a[i]) return false;}}; return true;}; function applyAction(){var ce = document.getElementById('container');generateCSS();}; function updateAction(){applyAction();}; function generateCSS(){var ce = document.getElementById('container'),f = document.forms[0],num = new RegExp('[0-9]+','g'),s,t;ce.style.cssText = ""; if(f.text_font.value)ce.style.fontFamily = f.text_font.value; if(f.text_size.value)ce.style.fontSize = f.text_size.value + (isNum(f.text_size.value) ? f.text_size_measurement.value : ""); if(f.text_style.value)ce.style.fontStyle = f.text_style.value; if(f.text_lineheight.value)ce.style.lineHeight = f.text_lineheight.value + (isNum(f.text_lineheight.value) ? f.text_lineheight_measurement.value : ""); if(f.text_case.value)ce.style.textTransform = f.text_case.value; if(f.text_weight.value)ce.style.fontWeight = f.text_weight.value; if(f.text_variant.value)ce.style.fontVariant = f.text_variant.value; if(document.getElementById('text_color').style.backgroundColor && document.getElementById('text_color').style.backgroundColor !='transparent')ce.style.color = document.getElementById('text_color').style.backgroundColor;s = "";s += f.text_underline.checked ? " underline" : "";s += f.text_overline.checked ? " overline" : "";s += f.text_linethrough.checked ? " line-through" : "";s += f.text_blink.checked ? " blink" : "";s = s.length > 0 ? s.substring(1) : s; if (f.text_none.checked)s = "none";ce.style.textDecoration = s; if(document.getElementById('background_color').style.backgroundColor != 'transparent')ce.style.backgroundColor = document.getElementById('background_color').style.backgroundColor; if(f.background_image.value)ce.style.backgroundImage = f.background_image.value != "" ? "url(" + f.background_image.value + ")" : ""; if(f.background_repeat.value)ce.style.backgroundRepeat = f.background_repeat.value; if(f.background_attachment.value)ce.style.backgroundAttachment = f.background_attachment.value; if (f.background_hpos.value != "" && f.background_hpos.value){s = "";s += f.background_hpos.value + (isNum(f.background_hpos.value) ? f.background_hpos_measurement.value : "") + " ";s += f.background_vpos.value + (isNum(f.background_vpos.value) ? f.background_vpos_measurement.value : "");ce.style.backgroundPosition = s;}; if(f.block_wordspacing.value)ce.style.wordSpacing = f.block_wordspacing.value + (isNum(f.block_wordspacing.value) ? f.block_wordspacing_measurement.value : ""); if(f.block_letterspacing.value)ce.style.letterSpacing = f.block_letterspacing.value + (isNum(f.block_letterspacing.value) ? f.block_letterspacing_measurement.value : ""); if(f.block_vertical_alignment.value)ce.style.verticalAlign = f.block_vertical_alignment.value; if(f.block_text_align.value)ce.style.textAlign = f.block_text_align.value; if(f.block_text_align.value)ce.style.textIndent = f.block_text_indent.value + (isNum(f.block_text_indent.value) ? f.block_text_indent_measurement.value : ""); if(f.block_whitespace.value)ce.style.whiteSpace = f.block_whitespace.value; if(f.block_display.value)ce.style.display = f.block_display.value; if(f.box_width.value)ce.style.width = f.box_width.value + (isNum(f.box_width.value) ? f.box_width_measurement.value : ""); if(f.box_height.value)ce.style.height = f.box_height.value + (isNum(f.box_height.value) ? f.box_height_measurement.value : ""); if(f.box_float.value)ce.style.styleFloat = f.box_float.value; if (!document.all)ce.style.cssFloat = f.box_float.value; if(f.box_clear.value)ce.style.clear = f.box_clear.value; if (!f.box_padding_same.checked){if(f.box_padding_top.value)ce.style.paddingTop = f.box_padding_top.value + (isNum(f.box_padding_top.value) ? f.box_padding_top_measurement.value : ""); if(f.box_padding_right.value)ce.style.paddingRight = f.box_padding_right.value + (isNum(f.box_padding_right.value) ? f.box_padding_right_measurement.value : ""); if(f.box_padding_bottom.value)ce.style.paddingBottom = f.box_padding_bottom.value + (isNum(f.box_padding_bottom.value) ? f.box_padding_bottom_measurement.value : ""); if(f.box_padding_left.value)ce.style.paddingLeft = f.box_padding_left.value + (isNum(f.box_padding_left.value) ? f.box_padding_left_measurement.value : "");}else{if(f.box_padding_top.value)ce.style.padding = f.box_padding_top.value + (isNum(f.box_padding_top.value) ? f.box_padding_top_measurement.value : "");}; if (!f.box_margin_same.checked){if(f.box_padding_top.value)ce.style.marginTop = f.box_margin_top.value + (isNum(f.box_margin_top.value) ? f.box_margin_top_measurement.value : ""); if(f.box_margin_right.value)ce.style.marginRight = f.box_margin_right.value + (isNum(f.box_margin_right.value) ? f.box_margin_right_measurement.value : ""); if(f.box_margin_bottom.value )ce.style.marginBottom = f.box_margin_bottom.value + (isNum(f.box_margin_bottom.value) ? f.box_margin_bottom_measurement.value : ""); if(f.box_margin_left.value)ce.style.marginLeft = f.box_margin_left.value + (isNum(f.box_margin_left.value) ? f.box_margin_left_measurement.value : "");}else{if(f.box_margin_top.value)ce.style.margin = f.box_margin_top.value + (isNum(f.box_margin_top.value) ? f.box_margin_top_measurement.value : "");}; if (!f.border_style_same.checked){if(document.getElementById('border_style_top').value)ce.style.borderTopStyle = document.getElementById('border_style_top').value; if(document.getElementById('border_style_right').value)ce.style.borderRightStyle = document.getElementById('border_style_right').value; if(document.getElementById('border_style_bottom').value)ce.style.borderBottomStyle = document.getElementById('border_style_bottom').value; if(document.getElementById('border_style_left').value)ce.style.borderLeftStyle = document.getElementById('border_style_left').value;}else{if(document.getElementById('border_style_top').value)ce.style.borderStyle = document.getElementById('border_style_top').value;}; if (!f.border_width_same.checked){if(f.border_width_top.value)ce.style.borderTopWidth = f.border_width_top.value + (isNum(f.border_width_top.value) ? f.border_width_top_measurement.value : ""); if(f.border_width_right.value)ce.style.borderRightWidth = f.border_width_right.value + (isNum(f.border_width_right.value) ? f.border_width_right_measurement.value : ""); if(f.border_width_bottom.value)ce.style.borderBottomWidth = f.border_width_bottom.value + (isNum(f.border_width_bottom.value) ? f.border_width_bottom_measurement.value : ""); if(f.border_width_left.value)ce.style.borderLeftWidth = f.border_width_left.value + (isNum(f.border_width_left.value) ? f.border_width_left_measurement.value : "");}else{if(f.border_width_top.value)ce.style.borderWidth = f.border_width_top.value;}; if (!f.border_color_same.checked){if(document.getElementById('border_color_top').style.backgroundColor != 'transparent')ce.style.borderTopColor = document.getElementById('border_color_top').style.backgroundColor; if(document.getElementById('border_color_right').style.backgroundColor != 'transparent')ce.style.borderRightColor = document.getElementById('border_color_right').style.backgroundColor; if(document.getElementById('border_color_bottom').style.backgroundColor != 'transparent')ce.style.borderBottomColor = document.getElementById('border_color_bottom').style.backgroundColor; if(document.getElementById('border_color_left').style.backgroundColor != 'transparent')ce.style.borderLeftColor = document.getElementById('border_color_left').style.backgroundColor;}else{if(document.getElementById('border_color_top').style.backgroundColor != 'transparent')ce.style.borderColor = document.getElementById('border_color_top').style.backgroundColor;}; if(f.list_type.value)ce.style.listStyleType = f.list_type.value; if(f.list_position.value)ce.style.listStylePosition = f.list_position.value; if(f.list_bullet_image.value)ce.style.listStyleImage = f.list_bullet_image.value != "" ? "url(" + f.list_bullet_image.value + ")" : ""; if(ce.style.position = f.positioning_type.value)ce.style.position = f.positioning_type.value; if(ce.style.position = f.positioning_type.value)ce.style.visibility = f.positioning_visibility.value; if (ce.style.width == "")ce.style.width = f.positioning_width.value + (isNum(f.positioning_width.value) ? f.positioning_width_measurement.value : ""); if (ce.style.height == "")ce.style.height = f.positioning_height.value + (isNum(f.positioning_height.value) ? f.positioning_height_measurement.value : ""); if(f.positioning_zindex.value)ce.style.zIndex = f.positioning_zindex.value; if(f.positioning_overflow.value)ce.style.overflow = f.positioning_overflow.value; if (!f.positioning_placement_same.checked){if(f.positioning_placement_top.value )ce.style.top = f.positioning_placement_top.value + (isNum(f.positioning_placement_top.value) ? f.positioning_placement_top_measurement.value : ""); if(positioning_placement_right.value )ce.style.right = f.positioning_placement_right.value + (isNum(f.positioning_placement_right.value) ? f.positioning_placement_right_measurement.value : ""); if(f.positioning_placement_bottom.value)ce.style.bottom = f.positioning_placement_bottom.value + (isNum(f.positioning_placement_bottom.value) ? f.positioning_placement_bottom_measurement.value : ""); if(f.positioning_placement_left.value)ce.style.left = f.positioning_placement_left.value + (isNum(f.positioning_placement_left.value) ? f.positioning_placement_left_measurement.value : "");} else{if(f.positioning_placement_top.value){s = f.positioning_placement_top.value + (isNum(f.positioning_placement_top.value) ? f.positioning_placement_top_measurement.value : "");ce.style.top = s;ce.style.right = s;ce.style.bottom = s;ce.style.left = s;}}; if (!f.positioning_clip_same.checked){s = null; if(f.positioning_clip_top.value);s = (isNum(f.positioning_clip_top.value)  ? f.positioning_clip_top.value + f.positioning_clip_top_measurement.value : "auto") + " "; if(f.positioning_clip_right.value)s += (isNum(f.positioning_clip_right.value) ? f.positioning_clip_right.value + f.positioning_clip_right_measurement.value : "auto") + " "; if(f.positioning_clip_bottom.value)s += (isNum(f.positioning_clip_bottom.value) ? f.positioning_clip_bottom.value + f.positioning_clip_bottom_measurement.value : "auto") + " "; if(f.positioning_clip_left.value)s += (isNum(f.positioning_clip_left.value) ? f.positioning_clip_left.value + f.positioning_clip_left_measurement.value : "auto"); if(s){s = "rect(" + s + ")"; if (s != "rect(auto auto auto auto)")ce.style.clip = s;}} else{if(f.positioning_clip_top.value){s = "rect(";t = isNum(f.positioning_clip_top.value) ? f.positioning_clip_top.value + f.positioning_clip_top_measurement.value : "auto";s += t + " ";s += t + " ";s += t + " ";s += t + ")"; if (s != "rect(auto auto auto auto)")ce.style.clip = s;}}; return ce.style.cssText.toLowerCase();}; function isNum(s){return new RegExp('[0-9]+','g').test(s);}; function showDisabledControls(){var f = document.forms,i,a; for (i=0;i<f.length;i++){for (a=0;a<f[i].elements.length;a++){if (f[i].elements[a].disabled);}}}; function fillSelect(f,s,param,dval,sep,em){var i,ar,p,se;f = document.forms[f];sep = typeof(sep) == "undefined" ? ";" : sep; if (em)addSelectValue(f,s,"","");ar = document.getElementById(f).value; for (i=0;i<ar.length;i++){se = false; if (ar[i].charAt(0) == '+'){ar[i] = ar[i].substring(1);se = true;};p = ar[i].split('='); if (p.length > 1){addSelectValue(f,s,p[0],p[1]); if (se)selectByValue(f,s,p[1]);}else{addSelectValue(f,s,p[0],p[0]); if (se)selectByValue(f,s,p[0]);}}}; function toggleSame(ce,pre){var el = document.forms[0].elements,i; if (ce.checked){document.getElementById(pre + "_top").disabled = false; document.getElementById(pre + "_right").disabled = true; document.getElementById(pre + "_right").style.background = document.getElementById(pre + "_top").style.backgroundColor; document.getElementById(pre + "_bottom").disabled = true; document.getElementById(pre + "_bottom").style.background = document.getElementById(pre + "_top").style.backgroundColor; document.getElementById(pre + "_left").disabled = true; document.getElementById(pre + "_left").style.background = document.getElementById(pre + "_top").style.backgroundColor; if (el[pre + "_top_measurement"]){el[pre + "_top_measurement"].disabled = false;el[pre + "_right_measurement"].disabled = true;el[pre + "_bottom_measurement"].disabled = true;el[pre + "_left_measurement"].disabled = true;}}else{document.getElementById(pre + "_top").disabled = false; document.getElementById(pre + "_right").disabled = false; document.getElementById(pre + "_bottom").disabled = false; document.getElementById(pre + "_left").disabled = false; if (el[pre + "_top_measurement"]){el[pre + "_top_measurement"].disabled = false;el[pre + "_right_measurement"].disabled = false;el[pre + "_bottom_measurement"].disabled = false;el[pre + "_left_measurement"].disabled = false;}};showDisabledControls();}; function synch(fr,to){var f = document.forms[0];f.elements[to].value = f.elements[fr].value; if (f.elements[fr + "_measurement"])selectByValue(f,to + "_measurement",f.elements[fr + "_measurement"].value);}; function UCFirstLetter( s ){s = ' ' + s; var result = ( /\s[a-z]/.test( s ) ) ?s.replace( /( )([a-z])/g,function(t,a,b){return ' ' + b.toUpperCase();}).substring(1) : s.substring(1);result = ( /("|')[a-z]/.test( result ) ) ?result.replace( /('|")([a-z])/g,function(t,a,b){return "'" + b.toUpperCase();}) : result;result = ( /,("|')[a-zA-Z]/.test( result ) ) ?result.replace( /(,'|,")([a-zA-Z])/g,function(t,a,b){return ",'" + b.toUpperCase();}) : result; return result.replace(/("|&quot;)/g,"'");};