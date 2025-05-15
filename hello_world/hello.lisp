;; sbcl --script hello.lisp

(print (list 

;; (defstruct (node (:print-function
;;                   (lambda (n s d)
;;                     (format s "#<~A>" (node-elt n)))))
;;   elt (l nil) (r n8il))
;;
;; (node-elt (make-node :elt 10))
;;
;; (elt (list 1 2 3) 1)
;;
;; (adjoin 'b '(a b c)) 
;;
;; (adjoin 'z '(a b c))
;;
;; (setf hf (make-hash-table))
;; (setf (gethash 'color hf) 'red)
;; (gethash 'color hf)
;;
;; (setf ht (make-hash-table))
;; (setf (gethash 'shape ht) 'sphe
;;       (gethash 'size ht) 'giant)
;; (maphash #'(lambda (k v)
;;                (format t "~A = ~A~%" k v))
;;            ht)
;;
;; (setf writers (make-hash-table :test #'equal :size 5))

(progn 
  (format t "a")
  (format t "b"))

(block head
       (format t "here we go.")
       (return-from head 'idea)
       (format t "we'll never see this"))

(block nil 
       (return 27))

(dolist (x '(a b c d e))
  (format t "~A " x)
  (if (eql x 'c)
    (return 'done)))

(defun foo ()
  (return-from foo 27))

(dotimes (i 10)
  (format t "~A " i))

(defun month-length (mon)
  (case mon
    ((jan mar may jul aug oct dec) 31)
    ((apr jun sept nov) 30)
    (feb (if (leap-year) 29 28))
    (otherwise "unknown month")))

(month-length 'jan)

(defun show-squares (start end)
  (do ((i start (+ i 1)))
    ((> i end) 'done)
    (format t "~A ~A~%" i (* i i))))
(show-squares 0 10)

(dotimes (x 5 x)
  (format t "~A " x))


(defun factorial (n)
  (do ((j n (- j 1))
       (f 1 (* j f)))
    ((= j 0) f)
    (format t "-- ~A ~A --" j f)
    ))

(factorial 5)

(values 'a nil (+ 1 2))

(multiple-value-bind (x y z) (values 1 2 3)
  (list x y z))

(multiple-value-bind (s m h) (get-decoded-time)
  (format t "~A:~A:~A" h m s))

#(1 2 3 4 5)

(defun leap? (y)
  (and (zerop (mod y 4))
       (or (zerop (mod y 400))
           (not (zerop (mod y 100))))))

(mapcar #'leap? '(1994 1900 1600 2024))

(svref #(1 2 3 4 5) 2)
(svref (make-array 5) 2)
(elt (list 1 2 3 4 5) 2)

(list #(1 2 3) (vector 1 2 3))

(append '(a b c) '(b c d) '(e f) '(f) 'g)

(setf fn (let ((i 3))
           #'(lambda (x) (+ x i))))
(funcall fn 2)

(defun add-to-list (num lst) 
  (mapcar #'(lambda(x) (+ x num))
          lst))
))



