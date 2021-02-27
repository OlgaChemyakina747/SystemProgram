#!/bin/bash
echo " "
echo "Olga Chemyakina"
echo "File information"
echo "Checking the file for existence and displaying information about the file"
namefile=""
choice="yes"

	while true
        do
          	echo "Enter the file name: "
                read namefile
                if [ -e $namefile ]
                then
                    	echo ""
                        echo "File information: "
                        echo "File name: "; stat -c%n $namefile
                        echo "Type file: "; stat -c%F $namefile
                        echo "Size file: "; stat -c%s $namefile
                        echo "Owner file: "; stat -c%U $namefile
                        echo "Access rights: "; stat -c%A $namefile
                        echo "File creation date: "; stat -c%w $namefile
                        echo ""
                else
                    	echo "The requested file does not exist" >/dev/stderr
                        echo ""
                fi
                echo "Want to find the file again? (y\N)"
        while true
        do
          	read choice
                case "$choice" in
                        "y"|"yes"|"Y"|"Yes") echo ""; break;;
                        "n"|"no"|"N"|"No") echo ""; echo "Goodbye!"; exit $?;;
                        * ) echo "Error :( You need to enter y or N" >/dev/stderr;;
                esac
        done
done
