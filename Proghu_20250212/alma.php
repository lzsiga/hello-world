<?php
  /* htmlspecialchars is called all the time */
  function hs($s) {
    return htmlspecialchars($s, ENT_QUOTES, 'UTF-8');
  }

  ini_set('display_errors', '1');
  error_reporting(E_ALL);

##header('Content-Type: text/html; charset=UTF-8');
  header('Content-type: application/json; charset=UTF-8');

  $reply= array('nev'=>'', 'jelszo'=>'', 'egyutt'=>'');

  $fusr= isset($_POST['nev']);
  $fpwd= isset($_POST['jelszo']);

  if ($fusr) {
      $reply['nev'] = sprintf ('Oke, username=%s<br>', hs($_POST['nev']));
      $reply['nev'].= sprintf ('Forditottja=%s<br>', hs(strrev($_POST['nev'])));
  }
  if ($fpwd) {
      $reply['jelszo'] = sprintf ('Oke, jelszo=%s<br>', hs($_POST['jelszo']));
      $reply['jelszo'].= sprintf ('Forditottja=%s<br>', hs(strrev($_POST['jelszo'])));
  }

  if ($fusr && $fpwd) {
      $reply['egyutt'] = "Mivel 'nev' es 'jelszo' is van,<br>";
      $reply['egyutt'].= "megpróbállak bejelentkeztetni<br>";
      if (rand(1, 100) <= 60) $reply['egyutt'].= "Sikerült (erre 60% esély volt)";
      else                    $reply['egyutt'].= "Nem sikerült (erre 40% esély volt)";
  }

##var_dump($reply);
  print json_encode ($reply);
?>
