#!/bin/bash
 
function shutdown()
{
        date
        echo "Shutting down Monit"
		killall monit
}
 
date
echo "Starting Monit"
 
#. $CATALINA_HOME/bin/catalina.sh start
 
# Allow any signal which would kill a process to stop Monit
trap shutdown HUP INT QUIT ABRT KILL ALRM TERM TSTP
