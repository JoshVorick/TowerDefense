Tower_Defense
=============
This is going to be an open-source **Linux** game! Hooray! I will be using **SDL** as my wonderful gaming library.

Currently, the goal is to (eventually) have a TD game that has multiple maps going at once. You would have to manage all of them as waves come simoultaneously. You could send resources between the maps (each map would be its own city/planet/area) but there would be a time delay and possibly a shipping cost. On each map the waves would fluctuate differently and towers costs would be different. Resources in this game will be colors (i.e. RGB). By buying your towers with different ratios of colors, you will get towers with slightly different stats (and of different colors). Hopefully this will be fun and all. Just like sleep.

###Screenshot!
![screen1](https://raw2.github.com/TheBananaWhale/Tower_Defense/master/img/Screenshot.png)

How To Play:
============
Right now there aren't really levels, so it is an uber sandbox game. The objective is to kill the enemies! You can place towers of different shapes and colors to try and stop them. They will come from the top and try and get to the bottom.
### Things to know:
*  That little black box is your **selector**. Use it to place new towers.
*  The red, blue, and green is your 'money.' You buy towers using these resources. You get more when you kill an enemy.
*  "Cost for selected Tower" is the how much red, green, and blue the next tower will cost based on your currently chosen.
*  You get control over the color of your next tower. The color of your cursor is like a preview of your next tower's color.
* Enemies will walk around your towers, so you can build a maze. Its a-maze-ing!
* "src/defs.h" has the stats for all the towers and enemies.

### CONTROLS
* **HJKL** - Move your cursor
* **Q/W/E** - Increase R/G/B ratio respectively. This will influence the ratio of R:G:B spent on the next tower, and its color! Press **Shift** to increase these values faster.
* **A/S/D** - Decrease R/G/B ratio respectively. Press **Shift** to decrease these values faster.
* **1** - Select **Triangle**. If you press 1, your next tower will be triangle shaped (weakest and cheapest tower)
* **2** - Select **Square**. If you press 2, your next tower will be square shaped
* **3** - Select **Pentagon**. If you press 3, your next tower will be pentagon shaped
* **4** - Select **Hexagon**. If you press 4, your next tower will be hexagon shaped (strongest and most expensive tower)
* **Space** - Place a tower. You cannot place towers ontop of eachother, nor in a way that blocks the enemy's path.

Install
=======
1.  To install, you will need to have installed the following: **gcc, SDL, SDL_image, SDL_mixer, and SDL_ttf**
2.  Then, just **make**, run, and you're ready to have fun!
