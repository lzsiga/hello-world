<?php
  ini_set('display_errors', '1');
  error_reporting(E_ALL);

  header('Content-Type: text/html; charset=UTF-8');

/*
  printf ('<p>%s:%s var_dump($_POST):</p>', basename(__FILE__), __LINE__);
  print ("<pre>");
  var_dump ($_POST);
  print ("</pre>\n");
*/
  $username = $_POST['nev'];
  $password = $_POST['jelszo'];

  printf ("username='%s'<br>", htmlspecialchars($username, ENT_QUOTES, "UTF-8"));
  printf ("password='%s'<br>", htmlspecialchars($password, ENT_QUOTES, "UTF-8"));

  $username = strrev($username); // megforditja a kiírás sorrendjét
  $password = strrev($password); // megforditja a kiírás sorrendjét

  printf ("fordított username='%s'<br>", htmlspecialchars($username, ENT_QUOTES, "UTF-8"));
  printf ("fordított password='%s'<br>", htmlspecialchars($password, ENT_QUOTES, "UTF-8"));

  $username = "Hibás név!";
  $password = "Hibás jelszó";

  printf ("hibaüzenet1='%s'<br>", htmlspecialchars($username, ENT_QUOTES, "UTF-8"));
  printf ("hibaüzenet2='%s'<br>", htmlspecialchars($password, ENT_QUOTES, "UTF-8"));
?>
