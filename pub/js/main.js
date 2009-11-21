$(document).ready(function(){
  Quode.JSH.highlight("code");
  $("#previewb").click(function(){
    previewComment();
  });
  if (typeof aInit != "undefined") aInit();
});

function previewComment() {
  if (!jQuery.trim($("#com_con").val()).length) 
    return;
  $.ajax({
    type: "POST",
    url: "/preview",
    dataType: "text",
    data: [
     { name: 'user', value: $("#user").val() },
     { name: 'email', value: $("#email").val() },
     { name: 'site', value: $("#site").val() },
     { name: 'com_con', value: $("#com_con").val() }
   ],
    success: function(data){
      $("#preview-comment").html(data);
      //Quode.JSH.highlight(document.getElementById('preview-comment').getElementsByTagName('code'));
    },
    complete: function() {
    }
  });
  
  
}
