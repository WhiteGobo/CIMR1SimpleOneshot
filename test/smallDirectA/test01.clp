(defrule rule0 (declare (salience 0))
	=>
	(println "exit with success")
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#exit>)
)

(defrule rule1 (declare (salience 50))
	=>
	(println "exit with failure")
	(<http://WhiteGobo.github.io/CIMR1SimpleOneshot/commands#fail>)
)
