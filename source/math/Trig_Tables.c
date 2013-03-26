#define _GENERIC_ARCHIVE

#include "../X3D.h"

// Complete sine << 8 table using increments of about .703 degrees
const short X3D_SinTable[512]={
	0,3,6,9,12,15,18,21,25,28,31,34,37,40,43,46,49,53,56,59,62,65,68,71,74,77,80,83,86,89,92,95,
	97,100,103,106,109,112,115,117,120,123,126,128,131,134,136,139,142,144,147,149,152,155,157,159,162,164,167,169,171,174,176,178,
	181,183,185,187,189,191,193,195,197,199,201,203,205,207,209,211,212,214,216,217,219,221,222,224,225,227,228,230,231,232,234,235,
	236,237,238,239,241,242,243,244,244,245,246,247,248,249,249,250,251,251,252,252,253,253,254,254,254,255,255,255,255,255,255,255,
	256,255,255,255,255,255,255,255,254,254,254,253,253,252,252,251,251,250,249,249,248,247,246,245,244,244,243,242,241,239,238,237,
	236,235,234,232,231,230,228,227,225,224,222,221,219,217,216,214,212,211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,
	181,178,176,174,171,169,167,164,162,159,157,155,152,149,147,144,142,139,136,134,131,128,126,123,120,117,115,112,109,106,103,100,
	97,95,92,89,86,83,80,77,74,71,68,65,62,59,56,53,49,46,43,40,37,34,31,28,25,21,18,15,12,9,6,3,
	0,-3,-6,-9,-12,-15,-18,-21,-25,-28,-31,-34,-37,-40,-43,-46,-49,-53,-56,-59,-62,-65,-68,-71,-74,-77,-80,-83,-86,-89,-92,-95,
	-97,-100,-103,-106,-109,-112,-115,-117,-120,-123,-126,-128,-131,-134,-136,-139,-142,-144,-147,-149,-152,-155,-157,-159,-162,-164,-167,-169,-171,-174,-176,-178,
	-181,-183,-185,-187,-189,-191,-193,-195,-197,-199,-201,-203,-205,-207,-209,-211,-212,-214,-216,-217,-219,-221,-222,-224,-225,-227,-228,-230,-231,-232,-234,-235,
	-236,-237,-238,-239,-241,-242,-243,-244,-244,-245,-246,-247,-248,-249,-249,-250,-251,-251,-252,-252,-253,-253,-254,-254,-254,-255,-255,-255,-255,-255,-255,-255,
	-256,-255,-255,-255,-255,-255,-255,-255,-254,-254,-254,-253,-253,-252,-252,-251,-251,-250,-249,-249,-248,-247,-246,-245,-244,-244,-243,-242,-241,-239,-238,-237,
	-236,-235,-234,-232,-231,-230,-228,-227,-225,-224,-222,-221,-219,-217,-216,-214,-212,-211,-209,-207,-205,-203,-201,-199,-197,-195,-193,-191,-189,-187,-185,-183,
	-181,-178,-176,-174,-171,-169,-167,-164,-162,-159,-157,-155,-152,-149,-147,-144,-142,-139,-136,-134,-131,-128,-126,-123,-120,-117,-115,-112,-109,-106,-103,-100,
	-97,-95,-92,-89,-86,-83,-80,-77,-74,-71,-68,-65,-62,-59,-56,-53,-49,-46,-43,-40,-37,-34,-31,-28,-25,-21,-18,-15,-12,-9,-6,-3
};

// Complete tangent << 9 table using the same increments.  127, 129, 383, and 385 are truncated to fit within a signed word.
const short X3D_TanTable[512]={
	0,6,12,18,25,31,37,44,50,56,63,69,75,82,88,95,101,108,114,121,128,134,141,148,155,162,169,176,183,190,197,204,
	212,219,226,234,242,249,257,265,273,281,290,298,306,315,324,333,342,351,360,370,379,389,399,409,420,430,441,452,464,475,487,499,
	512,524,537,551,564,579,593,608,623,639,656,672,690,708,726,746,766,787,808,830,854,878,903,930,957,986,1017,1049,1082,1117,1155,1194,
	1236,1280,1327,1377,1430,1488,1549,1616,1687,1765,1850,1942,2044,2155,2280,2418,2573,2749,2950,3182,3451,3769,4151,4616,5198,5945,6941,8333,10421,13900,20856,32767,
	0,-32768,-20856,-13900,-10421,-8333,-6941,-5945,-5198,-4616,-4151,-3769,-3451,-3182,-2950,-2749,-2573,-2418,-2280,-2155,-2044,-1942,-1850,-1765,-1687,-1616,-1549,-1488,-1430,-1377,-1327,-1280,
	-1236,-1194,-1155,-1117,-1082,-1049,-1017,-986,-957,-930,-903,-878,-854,-830,-808,-787,-766,-746,-726,-708,-690,-672,-656,-639,-623,-608,-593,-579,-564,-551,-537,-524,
	-512,-499,-487,-475,-464,-452,-441,-430,-420,-409,-399,-389,-379,-370,-360,-351,-342,-333,-324,-315,-306,-298,-290,-281,-273,-265,-257,-249,-242,-234,-226,-219,
	-212,-204,-197,-190,-183,-176,-169,-162,-155,-148,-141,-134,-128,-121,-114,-108,-101,-95,-88,-82,-75,-69,-63,-56,-50,-44,-37,-31,-25,-18,-12,-6,
	0,6,12,18,25,31,37,44,50,56,63,69,75,82,88,95,101,108,114,121,128,134,141,148,155,162,169,176,183,190,197,204,
	212,219,226,234,242,249,257,265,273,281,290,298,306,315,324,333,342,351,360,370,379,389,399,409,420,430,441,452,464,475,487,499,
	512,524,537,551,564,579,593,608,623,639,656,672,690,708,726,746,766,787,808,830,854,878,903,930,957,986,1017,1049,1082,1117,1155,1194,
	1236,1280,1327,1377,1430,1488,1549,1616,1687,1765,1850,1942,2044,2155,2280,2418,2573,2749,2950,3182,3451,3769,4151,4616,5198,5945,6941,8333,10421,13900,20856,32767,
	0,-32768,-20856,-13900,-10421,-8333,-6941,-5945,-5198,-4616,-4151,-3769,-3451,-3182,-2950,-2749,-2573,-2418,-2280,-2155,-2044,-1942,-1850,-1765,-1687,-1616,-1549,-1488,-1430,-1377,-1327,-1280,
	-1236,-1194,-1155,-1117,-1082,-1049,-1017,-986,-957,-930,-903,-878,-854,-830,-808,-787,-766,-746,-726,-708,-690,-672,-656,-639,-623,-608,-593,-579,-564,-551,-537,-524,
	-512,-499,-487,-475,-464,-452,-441,-430,-420,-409,-399,-389,-379,-370,-360,-351,-342,-333,-324,-315,-306,-298,-290,-281,-273,-265,-257,-249,-242,-234,-226,-219,
	-212,-204,-197,-190,-183,-176,-169,-162,-155,-148,-141,-134,-128,-121,-114,-108,-101,-95,-88,-82,-75,-69,-63,-56,-50,-44,-37,-31,-25,-18,-12,-6
};