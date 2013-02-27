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

$sql = "INSERT INTO `Players` (`TeamNum`, `Entries`) VALUES ('".$_POST['TeamNum']."', '".$_POST['Entries']."')";
if (!mysql_query($sql,$con))
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Error: ' . $errorText);
}

mysql_close($con);

header('HTTP/1.1 200 ');
?>