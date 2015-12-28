#! /bin/sh

WHOAMI=`whoami`

PID=`ps -u $WHOAMI | grep MyDaemon | awk '{print $1}'`

if ( test "$1" = "" ) then
	echo "MyDaemon [start] [stop] [version]"
	exit 0
fi

if ( test "$1" = "status" ) then
	if ( test "$PID" = "" ) then
		echo "Not Running."
	else
		echo "Running."
	fi
	exit 0
fi


if ( test "$1" = "start" ) then
	if ( test "PID" = "" ) then
		MyDaemon
	fi
	exit 0
fi

if ( test "$1" = "stop" ) then
	if ( test "$PID" != "" ) then
		kill $PID
	fi
	exit 0
fi

if ( test "$1" = "version" ) then
	echo "version : 1.0.0"
	exit 0
fi

echo "MyDaemon [start] [stop] [version]"