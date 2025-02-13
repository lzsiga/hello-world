<?php
  $username = $_POST['nev'];
  $password = $_POST['jelszo'];
 
#---------Azt teszi amit szeretnék------------------------------------
  $username = strrev($username); // megforditja a kiírás sorrendjét
  $password = strrev($password); // megforditja a kiírás sorrendjét


  echo $username;
  echo $password;


#------------Ez a megoldás hibás----------------------------------------


# Ha valamilyen értéket rendelek a egyes vázozókhoz, (a fentiektől eltérően)   mind a kettőt kiírja  az index oldalon egymás után? Egy ellenőrzés lefuttatása után szeretném az eredményeket közölni. Ez csak egy lebutított próbálkozás. pld. az alábbi megoldás hibás!
/*
  $username = "Hibás név!";
  $password = "Hibás jelszó";


  echo $username;
  echo $password;
*/
?>
