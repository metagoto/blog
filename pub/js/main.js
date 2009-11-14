$(document).ready(function(){
  Quode.JSH.highlight("code");
  
  $("#previewb").click(function(){
    previewComment();
  });

  
});

function previewComment() {
  if (!jQuery.trim($("#com_con").val()).length) 
    return;
  //$("#preview-notifier").addClass('loading');
  //if (!$.browser.msie) $("#preview-comment").fadeTo("slow", 0.16);
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
      //$("#preview").removeClass('dispnone');
      $("#preview-comment").html(data);
      //Quode.JSH.highlight(document.getElementById('preview-comment').getElementsByTagName('code'));
    },
    complete: function() {
      //if (!$.browser.msie) $("#preview-comment").fadeTo("slow", 1);
      //$("#preview-notifier").removeClass('loading');
    }
  });
  
  
}
