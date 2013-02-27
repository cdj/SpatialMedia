<?php
include 'dbConnection.php';
$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
if (!$con)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Could not connect: ' . $errorText);
}
mysql_select_db($dbName, $con);

// No teams have won yet
$sql="UPDATE `Teams` SET `Wins`=(`Wins`-1) WHERE `Wins`>0";
$result = mysql_query($sql);
if (!$result)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Error: ' . $errorText);
}
mysql_close($con);
?>