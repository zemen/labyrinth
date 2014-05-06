<?php

//~ Debug
error_reporting(E_ALL);

//~ Handle get_md5 action
if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'get_md5') {
	$MD5 = 0;
	if (file_exists('files/messages'))
		$MD5 = md5_file('files/messages');
	echo $MD5;
	exit;
}

function create_user($username, &$user_list, $game_id) {
	$cookie_time = time() + 24*60*60;
	setcookie("username", $username, $cookie_time, '/');
	$_COOKIE['username'] = $username;
	setcookie("game_id", $game_id, $cookie_time, '/');
	$_COOKIE['game_id'] = $game_id;
	
	$new_id = count($user_list);
	array_push($user_list, $username);
	$handle = fopen('files/user_list', 'a');
	if (!$handle) {
		echo "Can't open file user_list\n";
		exit;
	}
	fprintf($handle, "%s\n", $username);
	fclose($handle);
	return $new_id;
}

function clear_cookies() {
	setcookie('username', null, time() - 96 * 3600, '/');
	unset($_COOKIE['username']);
	setcookie('messages_md5', null, time() - 96 * 3600, '/');
	unset($_COOKIE['messages_md5']);
	setcookie('game_id', null, time() - 96 * 3600, '/');
	unset($_COOKIE['game_id']);
}

$output = "";

//~ Handle clear game action
if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'clear_game') {
	$files = scandir('files/');
	foreach ($files as $file) {
		if ($file != "." && $file != "..") {
			$handle = fopen("files/" . $file, "w");
			fclose($handle);
		}
	}
	header("Location: index.php");
	exit;
}

//~ Initialize user list
$user_list = file("files/user_list", FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

//~ Get gameID
$game_info = file("files/info", FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
$gameID = false;
if (count($game_info) > 0)
	$gameID = $game_info[0];
if (!$gameID) {
	echo "Server is down, try later!\n";
	exit;
}

//~ Handle user logout
if (isset($_REQUEST['logout']) && (int) $_REQUEST['logout'] == 1) {
	clear_cookies();
	header("Location: index.php");
	exit;
}

//~ Try to login
$user_id = -1;
$username = "Nobody";
if (isset($_COOKIE['username']) && isset($_COOKIE['game_id'])) {
	$username = $_COOKIE['username'];
	if ($_COOKIE['game_id'] != $gameID || !in_array($username, $user_list)) {
		clear_cookies();
		$username = "Nobody";
	} else {
		$user_id = array_search($username, $user_list);
		$output .= "Hello, " . $username . "!\n";
	}
} else if (isset($_POST['username']) && $_POST['username'] != "") {
	$username = $_POST['username'];
	if (!in_array($username, $user_list)) {
		create_user($username, $user_list, $gameID);
		header("Location: index.php");
		exit;
	} else {
		$output .= "This username is in use\n";
		$username = "Nobody";
	}
}

//~ Print login or logout form
if ($user_id == -1) {
	ob_start();
	require_once('templates/login.html');
	$output .= ob_get_clean();
} else {
	ob_start();
	require_once('templates/logout.html');
	$output .= ob_get_clean();
}
	//~ $output .= '<a href="'.$_SERVER['PHP_SELF'].'?logout=1">Leave game</a><hr />';

//~ Set messages_md5
$MD5 = 0;
if (file_exists('files/messages'))
	$MD5 = md5_file('files/messages');
$cookie_time = time() + 24*60*60*30;
setcookie("messages_md5", $MD5, $cookie_time, '/');
$_COOKIE['messages_md5'] = $MD5;


// Javascript and CSS
$output .= '<link type="text/css" rel="stylesheet" href="templates/css/styles.css" />';
$output .= '<script type="text/javascript" src="js/jquery-1.4.2.min.js"></script>';
$output .= '<script type="text/javascript" src="js/main.js"></script>';

if ($user_id != -1) {
	//~ Handle message adding
	if (isset($_REQUEST['message'])) {
		$handle = fopen('files/' . $user_id, 'a');
		fprintf($handle, "%s\n", $_REQUEST['message']);
		fclose($handle);
		header("Location: index.php");
		exit;
	}
	//~ Print messages
	if (file_exists('files/messages')) {
		$messages = file('files/messages', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
		$found = false;
		foreach ($messages as $id => $message) {
			if ($message != "") {
				if (!$found)
					$output .= '<h3>Messages from server</h3><div class="message_box">';
				$output .= '<div class="message" id="message_'.$id.'" >' . $message . '</div>';
				$found = true;
			}
		}
		if ($found)
			$output .= '</div>';
	}
	ob_start();
	require_once('templates/enter_command.html');
	$output .= ob_get_clean();
}

echo $output;

require_once('templates/footer.html');

?>
