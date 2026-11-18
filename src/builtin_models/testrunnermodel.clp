;(deffacts startfail
;	(Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>)
;		(args)
;	)
;)


(defrule label_test
	(Frame
		;(object ?base)
		(slotkey <https://WhiteGobo.github.io/TestsuiteCrate/manifest#entries>)
		(slotvalue ?list))
	?testlist <- (AtomList (items $? ?x $?))
	(test (eq ?list ?testlist))
	=>
	(AssertAtom _:execute-test ?x)
)


(defrule load_subtest
	(Atom (op _:execute-test) (args ?x))
	=>
	(println "subload: " ?x)
	(<http://white.gobo/ClipsInfoModelRulemachine1#import> ?x "http://www.w3.org/ns/entailment/RDF")
)


(defrule execute-test
	(Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/input#script>) (args ?script))
	(Atom (op _:execute-test) (args ?x))
	(Member (instance ?x) (class <https://WhiteGobo.github.io/TestsuiteCrate/vocab#PositiveEntailmentTestCase>))
	(Frame (object ?x) (slotkey <http://schema.org/name>) (slotvalue ?name))
	(Frame (object ?x) (slotkey <http://schema.org/description>) (slotvalue ?description))
	(Frame (object ?x) (slotkey <https://WhiteGobo.github.io/TestsuiteCrate/manifest#premise>) (slotvalue ?premise))
	(Frame (object ?x) (slotkey <https://WhiteGobo.github.io/TestsuiteCrate/manifest#exit>) (slotvalue ?exit))
	=>
	(bind ?child (<http://white.gobo/new-blanknode>))
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#generate-child>
		?child ?script)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#add-deffacts>
		?child ?premise
	)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#add-end-condition>
		?child ?exit
	)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#run-rules> ?child)
	(bind ?status
		(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#get-status> ?child))
	(bind ?HasExpectedData
		(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#check-data>
		?child ?exit))
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#free-child> ?child)
	(AssertAtom <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#child-status>
		?x ?status ?HasExpectedData)
	(println "qwertz1 execute-test " ?name": "?description)
)

(defrule label-failed-test-wrong-data
	(Atom
		(op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#child-status>)
		(args ?x TRUE FALSE))
	=>
	(AssertAtom
		<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>
		?x "missing data")
	
)


(defrule label-failed-test-failure-status
	(Atom
		(op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#child-status>)
		(args ?x ?status ?))
	(test (not (or (eq ?status TRUE) (eq ?status <declined>))))
	=>
	(AssertAtom
		<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>
		?x ?status)
	
)


(defrule label-failed-test-supported-declined
	(Atom (op <supported-dialect>) (args ?dialect))
	(Atom
		(op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#child-status>)
		(args ?x <declined> ?))
	(Frame (object ?x) (slotkey <https://WhiteGobo.github.io/TestsuiteCrate/manifest#dialect>) (slotvalue ?dialect))
	=>
	(AssertAtom
		<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>
		?x "declined supported dialect")
)



(defrule ending-print-failures (declare (salience -1000))
	(Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>)
		(args ?x ?message $?))
	(Frame (object ?x) (slotkey <http://schema.org/name>) (slotvalue ?name))
	=>
	(<http://www.w3.org/2007/rif-builtin-action#print>
		"Failed test: " ?name" with: "?message"
"
	)
)

(defrule ending-fail-program (declare (salience -1001))
	(exists (Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>)
		(args $?x)
	))
	=>
	;(facts)
	(<http://www.w3.org/2007/rif-builtin-action#print>
		"Failure;"
	)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#fail>)
)


(defrule ending-fail-without-script (declare (salience -1001))
	(not (exists
		(Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/input#script>) (args ?))
	))
	=>
	;(facts)
	(<http://www.w3.org/2007/rif-builtin-action#print>
		"Failure; Missing script"
	)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#fail>)
)

(defrule ending-success-program (declare (salience -1001))
	(exists
		(Atom (op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/input#script>) (args ?script))
	)
	(not (exists (Atom
		(op <http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#failed-test>)
	)))
	=>
	;(facts)
	(<http://www.w3.org/2007/rif-builtin-action#print>
		"All test passed;"
	)
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#exit>)
)
