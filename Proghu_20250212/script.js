/* nested unnamed functions are cool
   for example, it's much harder to debug them
 */

function my_reply_received(reply_json) {
    $('#kiir1').html(reply_json.nev);
    $('#kiir2').html(reply_json.jelszo);
    $('#kiir3').html(reply_json.egyutt);
}

function my_click() {
    $.post('alma.php',{
      nev: $('#nev').val(),
      jelszo: $('#jelszo').val()
    },
    my_reply_received);
}

function my_ready() {
  $('#btn').click(my_click);
}

$(document).ready(my_ready);
