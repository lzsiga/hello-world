$(document).ready(function(){
  $('#btn').click(function(){
    $.post('alma.php',{
      nev: $('#nev').val(),
      jelszo: $('#jelszo').val()
    },
    function(data){
        $('#kiir2').html(data);
    });
  });
});
