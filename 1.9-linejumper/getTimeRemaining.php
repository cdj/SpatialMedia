<?php
// get remaining time in current round
include 'dbConnection.php';
$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
if (!$con)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Could not connect: ' . $errorText);
}
mysql_select_db($dbName, $con);

$sql="SELECT (`RoundLength`*1000 - TIMESTAMPDIFF(MICROSECOND, `RoundStartTime`, NOW())/1000) AS `timeRemaining` FROM `Settings`";
$result = mysql_query($sql);
if (!$result)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Error: ' . $errorText);
}
else
{
	$timeRemaining = mysql_result($result, 0);
	if ($timeRemaining < 0 || $timeRemaining === NULL) { $timeRemaining = 0; }
	echo round($timeRemaining);
}
mysql_close($con);
?>