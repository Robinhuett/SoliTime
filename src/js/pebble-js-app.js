var initialized = false;
var options = {};

Pebble.addEventListener("ready", function(e) {
	console.log("PblKit_called --- ready");
	initialized = true;
});

Pebble.addEventListener("showConfiguration", function(e) {
	console.log("PblKit_called --- showConfiguration");
	/*if (localStorage.getItem("localOptions") === null)
	{
		localStorage.setItem("localOptions", {"KEY_VIBRATEONBT":"on","KEY_SHOWDATE":"off","KEY_BGCOLOR":"gray"});
	}*/
	options = JSON.parse(localStorage.getItem("localOptions"));
	console.log('http://robinhuett.github.io/SoliTime/config.html' + '?' +
				'vibrateonbt_select=' + options.KEY_VIBRATEONBT + '&' + 
				'showdate_select=' + options.KEY_SHOWDATE + '&' + 
				'bgcolor_select=' + options.KEY_BGCOLOR);
	Pebble.openURL('http://robinhuett.github.io/SoliTime/config.html' + '?' +
				'vibrateonbt_select=' + options.KEY_VIBRATEONBT + '&' + 
				'showdate_select=' + options.KEY_SHOWDATE + '&' + 
				'bgcolor_select=' + options.KEY_BGCOLOR);
});

Pebble.addEventListener("webviewclosed", function(e) {
	console.log("PblKit_called --- webviewclosed");
	//Using primitive JSON validity and non-empty check
	if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
		options = JSON.parse(decodeURIComponent(e.response));
		console.log("Options = " + JSON.stringify(options));
		
		Pebble.sendAppMessage(options, 
			function(e) {
				console.log("Send_Settings --- sending");
				localStorage.setItem("localOptions", JSON.stringify(options));
			}, function(e) {
				console.log("Send_Settings --- failed");
			});
	} else {
		console.log("Cancelled");
	}
});