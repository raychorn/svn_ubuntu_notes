So you have a script of your own that you want to run at bootup, each time you boot up. This will tell you how to do that.

Write a script. put it in the /etc/init.d/ directory.
Lets say you called it FOO. You then run

% update-rc.d FOO defaults

You also have to make the file you created, FOO, executable, using
$chmod +x FOO

You can check out 
% man update-rc.d for more information. It is a Debian utility to install scripts. The option “defaults” puts a link to start FOO in run levels 2, 3, 4 and 5. (and puts a link to stop FOO into 0, 1 and 6.)
