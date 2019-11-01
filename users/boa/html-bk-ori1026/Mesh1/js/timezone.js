 var i18nLanguage=getCookie('userLanguage');
	function getCookie(name) {
	  var cookies = document.cookie;
	  var list = cookies.split("; ");          // 解析出名/值对列表
	      
	  for(var i = 0; i < list.length; i++) {
	    var arr = list[i].split("=");          // 解析出名和值
	    if(arr[0] == name)
	      return decodeURIComponent(arr[1]);   // 对cookie值解码
	  } 
	  return "";
	}
	
var ntp_zone_index = 51;
function ntp_entry(name, value) { 
	this.name = name ;
	this.value = value ;
}

var util_gw_array0,util_gw_array1,util_gw_array2,util_gw_array3,util_gw_array4,util_gw_array5,util_gw_array6,util_gw_array7,util_gw_array8,util_gw_array9,
util_gw_array20,util_gw_array21,util_gw_array22,util_gw_array23,util_gw_array24,util_gw_array25,util_gw_array26,util_gw_array27,util_gw_array28,util_gw_array29,
util_gw_array10,util_gw_array11,util_gw_array12,util_gw_array13,util_gw_array14,util_gw_array15,util_gw_array16,util_gw_array17,util_gw_array18,util_gw_array19,
util_gw_array30,util_gw_array31,util_gw_array32,util_gw_array33,util_gw_array34,util_gw_array35,util_gw_array36,util_gw_array37,util_gw_array38,util_gw_array39,
util_gw_array40,util_gw_array41,util_gw_array42,util_gw_array43,util_gw_array44,util_gw_array45,util_gw_array46,util_gw_array47,util_gw_array48,util_gw_array49,
util_gw_array50,util_gw_array51,util_gw_array52,util_gw_array53,util_gw_array54,util_gw_array55,util_gw_array56,util_gw_array57,util_gw_array58,util_gw_array59,
util_gw_array60,util_gw_array61,util_gw_array62,util_gw_array63,util_gw_array64;

if(i18nLanguage=='en'){
	util_gw_array0 = "(GMT-12:00)Eniwetok, Kwajalein";
	util_gw_array1 = "(GMT-11:00)Midway Island, Samoa";
	util_gw_array2 = "(GMT-10:00)Hawaii";
	util_gw_array3 = "(GMT-09:00)Alaska";
	util_gw_array4 = "(GMT-08:00)Pacific Time (US & Canada); Tijuana";
	util_gw_array5 = "(GMT-07:00)Arizona";
	util_gw_array6 = "(GMT-07:00)Mountain Time (US & Canada)";
	util_gw_array7 = "(GMT-06:00)Central Time (US & Canada)";
	util_gw_array8 = "(GMT-06:00)Mexico City, Tegucigalpa";
	util_gw_array9 = "(GMT-06:00)Saskatchewan";
	util_gw_array10 = "(GMT-05:00)Bogota, Lima, Quito";
	util_gw_array11 = "(GMT-05:00)Eastern Time (US & Canada)";
	util_gw_array12 = "(GMT-05:00)Indiana (East)";
	util_gw_array13 = "(GMT-04:00)Atlantic Time (Canada)";
	util_gw_array14 = "(GMT-04:00)Caracas, La Paz";
	util_gw_array15 = "(GMT-04:00)Santiago";
	util_gw_array16 = "(GMT-03:30)Newfoundland";
	util_gw_array17 = "(GMT-03:00)Brasilia";
	util_gw_array18 = "(GMT-03:00)Buenos Aires, Georgetown";
	util_gw_array19 = "(GMT-02:00)Mid-Atlantic";
	util_gw_array20 = "(GMT-01:00)Azores, Cape Verde Is.";
	util_gw_array21 = "(GMT)Casablanca, Monrovia";
	util_gw_array22 = "(GMT)Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London";
	util_gw_array23 = "(GMT+01:00)Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna";
	util_gw_array24 = "(GMT+01:00)Belgrade, Bratislava, Budapest, Ljubljana, Prague";
	util_gw_array25 = "(GMT+01:00)Barcelona, Madrid";
	util_gw_array26 = "(GMT+01:00)Brussels, Copenhagen, Madrid, Paris, Vilnius";
	util_gw_array27 = "(GMT+01:00)Paris";
	util_gw_array28 = "(GMT+01:00)Sarajevo, Skopje, Sofija, Warsaw, Zagreb";
	util_gw_array29 = "(GMT+02:00)Athens, Istanbul, Minsk";
	util_gw_array30 = "(GMT+02:00)Bucharest";
	util_gw_array31 = "(GMT+02:00)Cairo";
	util_gw_array32 = "(GMT+02:00)Harare, Pretoria";
	util_gw_array33 = "(GMT+02:00)Helsinki, Riga, Tallinn";
	util_gw_array34 = "(GMT+02:00)Jerusalem";
	util_gw_array35 = "(GMT+03:00)Baghdad, Kuwait, Riyadh";
	util_gw_array36 = "(GMT+03:00)Moscow, St. Petersburg, Volgograd";
	util_gw_array37 = "(GMT+03:00)Mairobi";
	util_gw_array38 = "(GMT+03:30)Tehran";
	util_gw_array39 = "(GMT+04:00)Abu Dhabi, Muscat";
	util_gw_array40 = "(GMT+04:00)Baku, Tbilisi";
	util_gw_array41 = "(GMT+04:30)Kabul";
	util_gw_array42 = "(GMT+05:00)Ekaterinburg";
	util_gw_array43 = "(GMT+05:00)Islamabad, Karachi, Tashkent";
	util_gw_array44 = "(GMT+05:30)Bombay, Calcutta, Madras, New Delhi";
	util_gw_array45 = "(GMT+06:00)Astana, Almaty, Dhaka";
	util_gw_array46 = "(GMT+06:00)Colombo";
	util_gw_array47 = "(GMT+07:00)Bangkok, Hanoi, Jakarta";
	util_gw_array48 = "(GMT+08:00)Beijing, Chongqing, Hong Kong, Urumqi";
	util_gw_array49 = "(GMT+08:00)Perth";
	util_gw_array50 = "(GMT+08:00)Singapore";
	util_gw_array51 = "(GMT+08:00)Taipei";
	util_gw_array52 = "(GMT+09:00)Osaka, Sapporo, Tokyo";
	util_gw_array53 = "(GMT+09:00)Seoul";
	util_gw_array54 = "(GMT+09:00)Yakutsk";
	util_gw_array55 = "(GMT+09:30)Adelaide";
	util_gw_array56 = "(GMT+09:30)Darwin";
	util_gw_array57 = "(GMT+10:00)Brisbane";
	util_gw_array58 = "(GMT+10:00)Canberra, Melbourne, Sydney";
	util_gw_array59 = "(GMT+10:00)Guam, Port Moresby";
	util_gw_array60 = "(GMT+10:00)Hobart";
	util_gw_array61 = "(GMT+10:00)Vladivostok";
	util_gw_array62 = "(GMT+11:00)Magadan, Solomon Is., New Caledonia";
	util_gw_array63 = "(GMT+12:00)Auckland, Wllington";
	util_gw_array64 = "(GMT+12:00)Fiji, Kamchatka, Marshall Is.";
}else{
	util_gw_array0 = "(GMT-12:00)埃尼威托克岛, 夸贾林环礁";
	util_gw_array1 = "(GMT-11:00)中途岛, 萨摩亚";
	util_gw_array2 = "(GMT-10:00)夏威夷";
	util_gw_array3 = "(GMT-09:00)阿拉斯加州";
	util_gw_array4 = "(GMT-08:00)太平洋时间(美国和加拿大); 提华纳";
	util_gw_array5 = "(GMT-07:00)亚利桑那州";
	util_gw_array6 = "(GMT-07:00)山地时区(美国和加拿大)";
	util_gw_array7 = "(GMT-06:00)中央时间(美国&加拿大)";
	util_gw_array8 = "(GMT-06:00)墨西哥城, 特古西加尔巴";
	util_gw_array9 = "(GMT-06:00)萨斯喀彻温省";
	util_gw_array10 = "(GMT-05:00)波哥大, 利马, 基多";
	util_gw_array11 = "(GMT-05:00)东部时间(美国&加拿大)";
	util_gw_array12 = "(GMT-05:00)印第安纳州(东)";
	util_gw_array13 = "(GMT-04:00)大西洋时间(加拿大)";
	util_gw_array14 = "(GMT-04:00)加拉加斯,拉巴斯";
	util_gw_array15 = "(GMT-04:00)圣地亚哥";
	util_gw_array16 = "(GMT-03:30)纽芬兰";
	util_gw_array17 = "(GMT-03:00)巴西利亚";
	util_gw_array18 = "(GMT-03:00)布宜诺斯艾利斯,乔治城";
	util_gw_array19 = "(GMT-02:00)大西洋中部";
	util_gw_array20 = "(GMT-01:00)亚速尔群岛,佛得角.";
	util_gw_array21 = "(GMT)卡萨布兰卡,蒙罗维亚";
	util_gw_array22 = "(GMT)格林威治标准时间:都柏林爱丁堡、里斯本、伦敦";
	util_gw_array23 = "(GMT+01:00)阿姆斯特丹、柏林、伯尔尼、罗马、斯德哥尔摩、维也纳";
	util_gw_array24 = "(GMT+01:00)贝尔格莱德,布拉迪斯拉发、布达佩斯、卢布尔雅那,布拉格";
	util_gw_array25 = "(GMT+01:00)巴萨,皇马";
	util_gw_array26 = "(GMT+01:00)布鲁塞尔,哥本哈根,马德里,巴黎,维尔纽斯";
	util_gw_array27 = "(GMT+01:00)巴黎";
	util_gw_array28 = "(GMT+01:00)萨拉热窝,斯科普里,索非亚,华沙,萨格勒布";
	util_gw_array29 = "(GMT+02:00)雅典,伊斯坦布尔,明斯克";
	util_gw_array30 = "(GMT+02:00)布加勒斯特";
	util_gw_array31 = "(GMT+02:00)开罗";
	util_gw_array32 = "(GMT+02:00)哈拉雷,比勒陀利亚";
	util_gw_array33 = "(GMT+02:00)赫尔辛基,包括里加、塔林";
	util_gw_array34 = "(GMT+02:00)耶路撒冷";
	util_gw_array35 = "(GMT+03:00)巴格达,科威特,利雅得";
	util_gw_array36 = "(GMT+03:00)莫斯科、圣彼得堡、伏尔加格勒";
	util_gw_array37 = "(GMT+03:00)内罗比";
	util_gw_array38 = "(GMT+03:30)德黑兰";
	util_gw_array39 = "(GMT+04:00)阿布扎比,马斯喀特";
	util_gw_array40 = "(GMT+04:00)巴库第比利斯";
	util_gw_array41 = "(GMT+04:30)喀布尔";
	util_gw_array42 = "(GMT+05:00)叶卡捷琳堡";
	util_gw_array43 = "(GMT+05:00)伊斯兰堡,巴基斯坦卡拉奇,塔什干";
	util_gw_array44 = "(GMT+05:30)孟买、加尔各答、马德拉斯,新德里";
	util_gw_array45 = "(GMT+06:00)阿斯塔纳、阿拉木图、达卡";
	util_gw_array46 = "(GMT+06:00)科伦坡";
	util_gw_array47 = "(GMT+07:00)曼谷、河内、雅加达";
	util_gw_array48 = "(GMT+08:00)北京、重庆、香港、乌鲁木齐";
	util_gw_array49 = "(GMT+08:00)珀斯";
	util_gw_array50 = "(GMT+08:00)新加坡";
	util_gw_array51 = "(GMT+08:00)台北";
	util_gw_array52 = "(GMT+09:00)大阪,札幌,东京";
	util_gw_array53 = "(GMT+09:00)首尔";
	util_gw_array54 = "(GMT+09:00)雅库茨克";
	util_gw_array55 = "(GMT+09:30)阿德莱德";
	util_gw_array56 = "(GMT+09:30)达尔文";
	util_gw_array57 = "(GMT+10:00)布里斯班";
	util_gw_array58 = "(GMT+10:00)堪培拉、墨尔本、悉尼";
	util_gw_array59 = "(GMT+10:00)关岛,莫尔兹比港";
	util_gw_array60 = "(GMT+10:00)霍巴特";
	util_gw_array61 = "(GMT+10:00)海参崴";
	util_gw_array62 = "(GMT+11:00)马加丹州,所罗门群岛。,新喀里多尼亚";
	util_gw_array63 = "(GMT+12:00)奥克兰, 惠灵顿";
	util_gw_array64 = "(GMT+12:00)斐济, 堪察加半岛, 马绍尔群岛.";
}



var ntp_zone_array=new Array(65);
ntp_zone_array[0]=new ntp_entry(util_gw_array0,"12 1");
ntp_zone_array[1]=new ntp_entry(util_gw_array1,"11 1");
ntp_zone_array[2]=new ntp_entry(util_gw_array2,"10 1");
ntp_zone_array[3]=new ntp_entry(util_gw_array3,"9 1");
ntp_zone_array[4]=new ntp_entry(util_gw_array4,"8 1");
ntp_zone_array[5]=new ntp_entry(util_gw_array5,"7 1");
ntp_zone_array[6]=new ntp_entry(util_gw_array6,"7 2");
ntp_zone_array[7]=new ntp_entry(util_gw_array7,"6 1");
ntp_zone_array[8]=new ntp_entry(util_gw_array8,"6 2");
ntp_zone_array[9]=new ntp_entry(util_gw_array9,"6 3");
ntp_zone_array[10]=new ntp_entry(util_gw_array10,"5 1");
ntp_zone_array[11]=new ntp_entry(util_gw_array11,"5 2");
ntp_zone_array[12]=new ntp_entry(util_gw_array12,"5 3");
ntp_zone_array[13]=new ntp_entry(util_gw_array13,"4 1");
ntp_zone_array[14]=new ntp_entry(util_gw_array14,"4 2");
ntp_zone_array[15]=new ntp_entry(util_gw_array15,"4 3");
ntp_zone_array[16]=new ntp_entry(util_gw_array16,"3 1");
ntp_zone_array[17]=new ntp_entry(util_gw_array17,"3 2");
ntp_zone_array[18]=new ntp_entry(util_gw_array18,"3 3");
ntp_zone_array[19]=new ntp_entry(util_gw_array19,"2 1");
ntp_zone_array[20]=new ntp_entry(util_gw_array20,"1 1");
ntp_zone_array[21]=new ntp_entry(util_gw_array21,"0 1");
ntp_zone_array[22]=new ntp_entry(util_gw_array22,"0 2");
ntp_zone_array[23]=new ntp_entry(util_gw_array23,"-1 1");
ntp_zone_array[24]=new ntp_entry(util_gw_array24,"-1 2");
ntp_zone_array[25]=new ntp_entry(util_gw_array25,"-1 3");
ntp_zone_array[26]=new ntp_entry(util_gw_array26,"-1 4");
ntp_zone_array[27]=new ntp_entry(util_gw_array27,"-1 5");
ntp_zone_array[28]=new ntp_entry(util_gw_array28,"-1 6");
ntp_zone_array[29]=new ntp_entry(util_gw_array29,"-2 1");
ntp_zone_array[30]=new ntp_entry(util_gw_array30,"-2 2");
ntp_zone_array[31]=new ntp_entry(util_gw_array31,"-2 3");
ntp_zone_array[32]=new ntp_entry(util_gw_array32,"-2 4");
ntp_zone_array[33]=new ntp_entry(util_gw_array33,"-2 5");
ntp_zone_array[34]=new ntp_entry(util_gw_array34,"-2 6");
ntp_zone_array[35]=new ntp_entry(util_gw_array35,"-3 1");
ntp_zone_array[36]=new ntp_entry(util_gw_array36,"-3 2");
ntp_zone_array[37]=new ntp_entry(util_gw_array37,"-3 3");
ntp_zone_array[38]=new ntp_entry(util_gw_array38,"-3 4");
ntp_zone_array[39]=new ntp_entry(util_gw_array39,"-4 1");
ntp_zone_array[40]=new ntp_entry(util_gw_array40,"-4 2");
ntp_zone_array[41]=new ntp_entry(util_gw_array41,"-4 3");
ntp_zone_array[42]=new ntp_entry(util_gw_array42,"-5 1");
ntp_zone_array[43]=new ntp_entry(util_gw_array43,"-5 2");
ntp_zone_array[44]=new ntp_entry(util_gw_array44,"-5 3");
ntp_zone_array[45]=new ntp_entry(util_gw_array45,"-6 1");
ntp_zone_array[46]=new ntp_entry(util_gw_array46,"-6 2");
ntp_zone_array[47]=new ntp_entry(util_gw_array47,"-7 1");
ntp_zone_array[48]=new ntp_entry(util_gw_array48,"-8 1");
ntp_zone_array[49]=new ntp_entry(util_gw_array49,"-8 2");
ntp_zone_array[50]=new ntp_entry(util_gw_array50,"-8 3");
ntp_zone_array[51]=new ntp_entry(util_gw_array51,"-8 4");
ntp_zone_array[52]=new ntp_entry(util_gw_array52,"-9 1");
ntp_zone_array[53]=new ntp_entry(util_gw_array53,"-9 2");
ntp_zone_array[54]=new ntp_entry(util_gw_array54,"-9 3");
ntp_zone_array[55]=new ntp_entry(util_gw_array55,"-9 4");
ntp_zone_array[56]=new ntp_entry(util_gw_array56,"-9 5");
ntp_zone_array[57]=new ntp_entry(util_gw_array57,"-10 1");
ntp_zone_array[58]=new ntp_entry(util_gw_array58,"-10 2");
ntp_zone_array[59]=new ntp_entry(util_gw_array59,"-10 3");
ntp_zone_array[60]=new ntp_entry(util_gw_array60,"-10 4");
ntp_zone_array[61]=new ntp_entry(util_gw_array61,"-10 5");
ntp_zone_array[62]=new ntp_entry(util_gw_array62,"-11 1");
ntp_zone_array[63]=new ntp_entry(util_gw_array63,"-12 1");
ntp_zone_array[64]=new ntp_entry(util_gw_array64,"-12 2");




function setTimeZone(field, value){
    field.selectedIndex = 4 ;
    for(i=0 ;i < field.options.length ; i++){
    	if(field.options[i].value == value){
		field.options[i].selected = true;
		break;
}
}

}