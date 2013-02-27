<?php 
$roundLength;
$waitingTime;
$roundStartTime;

function loadSettings()
{
	include 'dbConnection.php';
	global $roundLength, $waitingTime, $roundStartTime;
	$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
	if (!$con)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Could not connect: ' . $errorText);
	}
	mysql_select_db($dbName, $con);
	
	$sql="SELECT * FROM `Settings`";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	else
	{
		$row = mysql_fetch_array($result);
		$roundLength = $row['RoundLength'];
		$waitingTime = $row['WaitingTime'];
		$roundStartTime = $row['RoundStartTime'];
	}
	mysql_close($con);
}
?>