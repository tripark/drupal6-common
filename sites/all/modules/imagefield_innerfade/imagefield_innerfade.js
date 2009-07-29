Drupal.behaviors.imagefield_innerfade = function(context) {
  if ($('.imagefield_innerfade').length) {
    $('.imagefield_innerfade').innerfade({
      animationtype: Drupal.settings.imagefield_innerfade.animationtype,
    	speed: Drupal.settings.imagefield_innerfade.speed,
    	timeout: Drupal.settings.imagefield_innerfade.timeout,
    	type: Drupal.settings.imagefield_innerfade.type,
    	containerheight: Drupal.settings.imagefield_innerfade.containerheight,
    	runningclass: Drupal.settings.imagefield_innerfade.runningclass,
    	children: Drupal.settings.imagefield_innerfade.children,
    });
  }
};