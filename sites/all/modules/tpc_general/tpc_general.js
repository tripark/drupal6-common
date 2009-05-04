$(document).ready(function{
  // Traverse the DOM looking for any links with a title of "new-window" and
  // set a mouse click listener to open those links in a new window when
  // clicked.
  // This is done so that links in textile like the following can open
  // in a new window.
  // "example page(new-window)":http://example.com
  $("a[@title^='new-window']").each(function() {
    $(this).attr("title", '');
    $(this).click(function(){
      window.open(this.href);
      return false;
    });
  });
});