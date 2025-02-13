$(document).ready(function(){
  $('#btn').click(function(){
    //----------------------------------------------
    $.post('alma.php',{
      nev: $('#nev').val(),
    },
    function(data){
        $('#kiir1').html(data);
    });
    //-----------------------------------------------
    $.post('alma.php',{
     jelszo: $('#jelszo').val()
    },
    function(data){
        $('#kiir2').html(data);
    });
    //----------------------------------------------------
  });
});
