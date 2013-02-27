<?php
// get team info
include 'dbConnection.php';
$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
if (!$con)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Could not connect: ' . $errorText);
}
mysql_select_db($dbName, $con);

$sql="SELECT * FROM `Teams`";
$result = mysql_query($sql);
if (!$result)
{
	$errorText = mysql_error();
	header('HTTP/1.1 500 '.$errorText);
	die('Error: ' . $errorText);
}
else
{
	$rows = array();
	while($r = mysql_fetch_assoc($result))
	{
		$rows[] = $r;
	}
}
mysql_close($con);
header('Content-Type: application/json');
echo json_encode($rows);
?>