Menu Trails module implements an "active trail" in primary/secondary links, setting class="active" on parents to the current menu item.

It also allows nodes to be categorized by type or taxonomy as falling "under" existing menu items, triggering the above functionality.

To implement:

1) Install the module
2) Visit admin/settings/menutrails to classify nodes
3) Enjoy!

A NOTE TO THEMERS:

Menutrails.module contains a built-in override for theme_links, which is necessary to add the "active" class to the containing <li> for each menu item. If your theme already includes an override, this will cause a PHP error, and you will need to work out the differences between the two theme overrides yourself.

Feel free to suggest better alternatives or other ways to tackle the issue if you have them.
