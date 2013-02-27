<?php
$teamNumber;
$teamColor;

if($_GET['teamNum']===NULL) {
	$teamNumber = 1;
} else {
	$teamNumber = $_GET['teamNum'];
}

// get team color
include 'dbConnection.php';
$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
if (!$con)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Could not connect: ' . $errorText);
}
else
{
	mysql_select_db($dbName, $con);
	
	$sql="SELECT `Color` FROM `Teams` WHERE `Num` = " . $teamNumber;
	$result = mysql_query($sql);
	if (!$result)
	{
		$teamColor = "white";
	}
	else
	{
		$teamColor = mysql_result($result, 0);
	}
	mysql_close($con);
}
?>
<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<title>Line Jumper</title>
<link href="css/bootstrap.min.css" rel="stylesheet" type="text/css">
<script src="http://code.jquery.com/jquery-1.9.1.js"></script>
<script src="js/bootstrap.min.js"></script>
<script>
var teamNum = <?php echo $teamNumber; ?>;
var teamColor = "<?php echo $teamColor; ?>";
var timeRemaining = 0;
var timesPressed = 0;
var isRoundOver = true;
var refreshTimeRemainingTimer;
window.onload = function() {
	refreshTimeRemainingTimer = setInterval(function(){ getTimeRemaining() }, 250);
};

function getTimeRemaining() {
	$.get("getTimeRemaining.php", function(data) {
		timeRemaining = data;
	}).done(function() {
		if (timeRemaining <= 0) {
			$("#theButton").addClass("disabled");
			$("#theButton").removeClass("btn-primary");
			if (!isRoundOver) { // submit data: team, entries
				if (timesPressed > 0) {
					$.ajax({
						type: "POST",
						url: "submitPlayerData.php",
						data: { "TeamNum" : teamNum, "Entries" : timesPressed }
					 }).done(function() {
						 timesPressed = 0;
						 isRoundOver = true;
					 }).fail(function() {
						 console.log("Failed to submit player data");
					 });
				} else {
					isRoundOver = true;
				}
			}
		} else {
			$("#theButton").removeClass("disabled");
			$("#theButton").addClass("btn-primary");
			if (isRoundOver) { // reset round
				isRoundOver = false;
			}
		}
		$("#timeRemaining").text(timeRemaining/1000);
	}).fail(function() {
		timeRemaining = 0;
		isRoundOver = true;
	});
}

function buttonPressed() {
	if (!$("#theButton").hasClass("disabled")) {
		timesPressed++;
	}
}
</script>
</head>

<body style="background-color:<?php echo $teamColor; ?>">
<div style="position:absolute; right:50%; bottom:50%; text-align:center;">
<div style="position:relative; left:50%; top:50%;">
<p>Press the button as many times as possible before time is up!</p>
<button id="theButton" class="btn btn-large disabled" type="button" onMouseDown="buttonPressed()">Button</button>
<p>Time remaining:<br/><a id="timeRemaining">0</a> seconds</p>
</div>
</div>
</body>
</html>