#
# Change the HW variable to match the homework
#
NUM		= 0

HW  	= hw$(NUM)
CLASS	= /homes/cs240
PROJ	= $(CLASS)/public/homework/$(HW)

Makefile : $(PROJ)/Makefile
	rm -f Makefile
	cp $(PROJ)/Makefile .

#
# Extra targets to do interesting things...
#

submit:
	git checkout master >> .local.git.out || echo
	git add *.c *.h  >> .local.git.out || echo
	git commit -a -m "Commit homework $(HW)" >> .local.git.out || echo
	git push origin master

