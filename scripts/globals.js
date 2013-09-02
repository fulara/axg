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
