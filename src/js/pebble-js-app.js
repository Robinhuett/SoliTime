var initialized = false;
var options = {};

function GColorFromHex(hex) {
	var hexNum = parseInt(hex, 16);
	var a = 192;
	var r = (((hexNum >> 16) & 0xFF) >> 6) << 4;
	var g = (((hexNum >>  8) & 0xFF) >> 6) << 2;
	var b = (((hexNum >>  0) & 0xFF) >> 6) << 0;
	return a + r + g + b;
}
function GColorToHex(color) {
	var r = (color & 48) >> 4;
	var g = (color & 12) >> 2;
	var b = (color & 3) >> 0;
	var hexString = [r,g,b].map(function (x) {
		x *= 5;
		return x.toString(16) + x.toString(16);
	}).join('');
	return hexString.toUpperCase();
}

Pebble.addEventListener("ready", function(e) {
	console.log("PblKit_called --- ready");
	initialized = true;
});

Pebble.addEventListener("showConfiguration", function(e) {
	console.log("PblKit_called --- showConfiguration");
	var localoptions = JSON.parse(localStorage.getItem("localOptions"));
	console.log('http://robinhuett.github.io/SoliTime/config.html' + '?' +
				'vibrateonbt_select=' + localoptions.KEY_VIBRATEONBT + '&' + 
				'showdate_select=' + localoptions.KEY_SHOWDATE + '&' + 
				'boldhour_select=' + localoptions.KEY_BOLDHOUR + '&' + 
				'primarycolor_select=' + localoptions.KEY_PRIMARYCOLOR + '&' + 
				'secondarycolor_select=' + localoptions.KEY_SECONDARYCOLOR + '&' + 
				'backgroundcolor_select=' + localoptions.KEY_BACKGROUNDCOLOR);
	Pebble.openURL('http://robinhuett.github.io/SoliTime/config.html' + '?' +
				'vibrateonbt_select=' + localoptions.KEY_VIBRATEONBT + '&' + 
				'showdate_select=' + localoptions.KEY_SHOWDATE + '&' + 
				'boldhour_select=' + localoptions.KEY_BOLDHOUR + '&' + 
				'primarycolor_select=' + localoptions.KEY_PRIMARYCOLOR + '&' + 
				'secondarycolor_select=' + localoptions.KEY_SECONDARYCOLOR + '&' + 
				'backgroundcolor_select=' + localoptions.KEY_BACKGROUNDCOLOR);
});

Pebble.addEventListener("webviewclosed", function(e) {
	console.log("PblKit_called --- webviewclosed");
	//Using primitive JSON validity and non-empty check
	if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
		options = JSON.parse(decodeURIComponent(e.response));
		
		options.KEY_PRIMARYCOLOR = GColorFromHex(options.KEY_PRIMARYCOLOR.substring(2));
		options.KEY_SECONDARYCOLOR = GColorFromHex(options.KEY_SECONDARYCOLOR.substring(2));
		options.KEY_BACKGROUNDCOLOR = GColorFromHex(options.KEY_BACKGROUNDCOLOR.substring(2));
		
		Pebble.sendAppMessage(options, 
			function(e) {
				console.log("Send_Settings --- sending");
				
				options.KEY_PRIMARYCOLOR = '0x' + GColorToHex(options.KEY_PRIMARYCOLOR);
				options.KEY_SECONDARYCOLOR = '0x' + GColorToHex(options.KEY_SECONDARYCOLOR);
				options.KEY_BACKGROUNDCOLOR = '0x' + GColorToHex(options.KEY_BACKGROUNDCOLOR);
				
				localStorage.setItem("localOptions", JSON.stringify(options));
			}, function(e) {
				console.log("Send_Settings --- failed");
			});
	} else {
		console.log("Cancelled");
	}
});