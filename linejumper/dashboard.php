<?php
require_once('settings.php');
require_once('general.php');

resetGame();
loadSettings();
?>
<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<title>Line Jumper</title>
<style type="text/css">
.lineColumn {
	height: 100%;
	float:left;
	margin:0px;
	padding:0px;
}
.wins {
	font-size:60px;
	color:white;
	text-align:center;
}
body {
	padding:0px;
	font-family:Arial, Helvetica, sans-serif;
}
</style>
<script src="http://code.jquery.com/jquery-1.9.1.js"></script>
<script>
var roundLength = <?php echo $roundLength; ?>;
var waitingTime = <?php echo $waitingTime; ?>;
var teams;
// Timers
var lineWaitTimer;
var roundTimer;
var teamRefreshTimer;

window.onload = function() {
	$.get("getTeams.php", function(data) {
	  teams = data;
	}).done(function() {
		$.each(teams, function(index, value) {
			var bar = document.createElement("div");
			bar.setAttribute("id", "team" + value.Num);
			bar.setAttribute("class", "lineColumn");
			bar.setAttribute("style", "background-color:" + value.Color + "; width:" + 100/teams.length + "%");
			bar.innerHTML = "<p class=\"wins\"></p>";
			$("#barContainer").append(bar);
		});
	}).fail(function() { $("#barContainer").text("Error retreiving teams"); });
	lineWaitTimer = setInterval(function() { waitTimer() }, waitingTime*1000);
	roundTimer = setTimeout(function() { startRound() }, 4000);
	teamRefreshTimer = setInterval(function() { refreshTeamsInfo() }, 500);
};

function refreshTeamsInfo() {
	$.get("getTeams.php", function(data) {
	  teams = data;
	}).done(function() {
		$.each(teams, function(index, value) {
			var minutes = Math.floor((waitingTime-(value.Wins*10))/60);
			var seconds = Math.floor((waitingTime-(value.Wins*10))%60);
			var secondsString = "0";
			if (seconds<10) {
				secondsString += seconds;
			} else {
				secondsString = seconds;
			}
			$("#team" + value.Num + " .wins").html("Waiting time<br/>" 
													+ minutes
													+ ":" 
													+ secondsString);
		});
	});
}

function startRound() {
	// trigger start round on backend
	$.ajax("resetRound.php")
		.done(function() {
			// fire off timer to do end round
			roundTimer = setTimeout(function(){ endRound() }, (roundLength+5)*1000);
		}).fail(function() {
			console.log("Failed in starting round");
		});
}

function endRound() {
	$.ajax("endRound.php")
		.done(function() {
			// fire off timer to do end round
			roundTimer = setTimeout(function(){ startRound() }, (roundLength/2)*1000);
		}).fail(function() {
			console.log("Failed in ending round");
		});
}

function waitTimer() {
	$.ajax("winDecrement.php")
		.fail(function() {
			console.log("Failed in decrementing wins");
		});
}
</script>
</head>

<body>
<div id="barContainer" style="position:absolute; top:0px; bottom:0px; left:0px; right:0px;">
</div>
</body>
</html>
