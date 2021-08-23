
(cl:in-package :asdf)

(defsystem "deep_sort_pytorch-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Bound" :depends-on ("_package_Bound"))
    (:file "_package_Bound" :depends-on ("_package"))
    (:file "Bound" :depends-on ("_package_Bound"))
    (:file "_package_Bound" :depends-on ("_package"))
  ))