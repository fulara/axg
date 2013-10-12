var ctrlPressed = false;
$(window).keydown(function(evt) 
{
	if (evt.which == 17) 
	{
		ctrlPressed = true;
	}
	}).keyup(function(evt) {
	if (evt.which == 17) {
	ctrlPressed = false;
}});
$(document).ready(function() {
    var $dragging = null;
    $('body').on("mousedown", ".DraggableDiv", function(e) {
        $(this).attr('unselectable', 'on').addClass('draggable');
	var parId = $(this).attr('parentId');
	$(parId).addClass('workz');
        var el_w = $('.draggable').outerWidth(),
            el_h = $('.draggable').outerHeight();
        $('body').on("mousemove", function(e) {
            if ($dragging) {
		var parId = $dragging.attr('parentId');
		$par = $(parId);
		var dpos = $dragging.position();
		$par.offset({
                    top: e.pageY - dpos.top - el_h / 2,
                    left: e.pageX - dpos.left - el_w / 2
                });
            }
        });
        $dragging =$(e.target);
    }).on("mouseup", function(e) {
        $dragging = null;
        $(this).removeAttr('unselectable').removeClass('draggable');
    });
});
