
/**
 * Module dependencies.
 */

var express = require('express')
  , routes = require('./routes')
  , user = require('./routes/user')
  , http = require('http')
  , path = require('path')
  , Spacebrew = require('./sb-1.0.3')
  , Tumblr = require('tumblrwks');

var app = express();

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
  app.use(express.cookieParser('your secret here'));
  app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

var tumblrTag = "cat";
var latestPost = null;
var oldestTimestamp = 0;

var server = "sandbox.spacebrew.cc";
var name = "Tumblr";
var description = "A stream of Tumblr photo posts with the tag '"+tumblrTag+"'";
var sb = new Spacebrew.Spacebrew.Client( server, name, description );

sb.addPublish("PhotoUrl", "string", "The URL of a photo in a photo post");
sb.addPublish("BlogName", "string", "The name of a blog that posted using the tag '"+tumblrTag+"'");

sb.connect();

var tumblr = new Tumblr(
  {
/*
  You can get the consumerKey by registing a tumblr app: http://www.tumblr.com/oauth/apps
*/
    consumerKey: 'your consumer key'
  }
);


// Get 5 new posts every 15 seconds
// 	If we've seen the posts before, get older ones we haven't seen yet
// 	Send out blog names and photo URLs (if they are photo posts) to Spacebrew
setInterval(function(){
	var postsToGet = 5;
	console.log("======");
	console.log("--- Getting "+postsToGet+" posts ---");
	tumblr.get('/tagged', {tag: tumblrTag, filter: 'text', limit: postsToGet}, function(json){
		console.log("+++ "+json.length+" posts found +++");
		for (var i = 0; i < json.length; i++) {
			if (JSON.stringify(json[i]) == latestPost) {
				console.log("*** Reached latest post ***");
				break;
			}
			SendToSpacebrew(json[i]);
			postsToGet--;
		}
		latestPost = JSON.stringify(json[0]);
		if(oldestTimestamp == 0) { // first time running
			oldestTimestamp = json[json.length-1].timestamp;
		} else {
			if (postsToGet>0) {
				console.log("--- Getting "+postsToGet+" older posts ---");
				tumblr.get('/tagged', {tag: tumblrTag, filter: 'text', limit: postsToGet, before: oldestTimestamp}, function(json){
					console.log("+++ "+json.length+" older posts found +++");
					for (var i = 0; i < json.length; i++) {
						SendToSpacebrew(json[i]);
					}
					oldestTimestamp = json[json.length-1].timestamp;
				});
			}
		}
	});
}, 15000);

// Send out blog name and photo URL (if it is a photo post) to Spacebrew
function SendToSpacebrew(json) {
	if(sb._isConnected) {
		sb.send("BlogName", "string", json.blog_name);
		//console.log("* Blog = "+json.blog_name);

		if (json.type == "photo") {
			//console.log("* URL = "+json.photos[0].alt_sizes[0].url);
			sb.send("PhotoUrl", "string", json.photos[0].alt_sizes[0].url);
		} else {
			//console.log("** Not a photo; post type is "+json.type);
		}
	}
}