<?php 
// reset everything
function resetGame()
{
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
	$sql="UPDATE `Teams` SET `Wins`=0";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	
	// No recent rounds have been run
	$sql="UPDATE `Settings` SET `RoundStartTime`='0000-00-00 00:00:00'";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	
	// No entries for players yet
	$sql="TRUNCATE TABLE  `Players`";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	mysql_close($con);
}

function resetRound()
{
	include 'dbConnection.php';
	$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
	if (!$con)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Could not connect: ' . $errorText);
	}
	mysql_select_db($dbName, $con);
	
	// Round starts now
	$sql="UPDATE `Settings` SET `RoundStartTime`=NOW()";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	
	// No entries for players yet
	$sql="TRUNCATE TABLE  `Players`";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	mysql_close($con);
}


function endRound()
{
	include 'dbConnection.php';
	$con = mysql_connect($dbServerName,$dbUserName,$dbUserPassword);
	if (!$con)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Could not connect: ' . $errorText);
	}
	mysql_select_db($dbName, $con);
	
	// Find winning team
	$sql="SELECT `TeamNum`, AVG(`Entries`) AS `Score` FROM `Players` GROUP BY `TeamNum` ORDER BY `Score` DESC";
	$result = mysql_query($sql);
	if (!$result)
	{
		$errorText = mysql_error();
		header('HTTP/1.1 500 '.$errorText);
		die('Error: ' . $errorText);
	}
	$winner = mysql_fetch_assoc($result);
	
	if(mysql_num_rows($result) > 0)
	{
		// Record winner
		$topTeamNum = $winner["TeamNum"];
		$topNum = $winner["Score"];
		$secondNum = 0;
		if($winner = mysql_fetch_assoc($result))
		{
			$secondNum = $winner["Score"];
		}
		if($topNum > $secondNum) // is it a tie? if not, record winner
		{
			$sql="UPDATE `Teams` SET `Wins` = (`Wins` + 1) WHERE `Num` = '" . $topTeamNum ."'";
			$result = mysql_query($sql);
			if (!$result)
			{
				$errorText = mysql_error();
				header('HTTP/1.1 500 '.$errorText);
				die('Error: ' . $errorText);
			}
		}
		
		// Clear out entries
		$sql="TRUNCATE TABLE  `Players`";
		$result = mysql_query($sql);
		if (!$result)
		{
			$errorText = mysql_error();
			header('HTTP/1.1 500 '.$errorText);
			die('Error: ' . $errorText);
		}
	}
	mysql_close($con);
}
?>