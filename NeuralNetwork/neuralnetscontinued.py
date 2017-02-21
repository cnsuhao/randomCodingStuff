# neuralnetscontinued http://karpathy.github.io/neuralnets/
# Becoming a Backprop Ninja
import math

# variables
a = 1
b = 2
c = 3
d = 4
dx = 5
y = 10

# * gate
x = a * b
# and given gradient on x (dx),
# we saw that in backprop we would compute:
da = b * dx
db = a * dx

# + gate
x = a + b
# ->
da = 1.0 * dx
db = 1.0 * dx

# lets compute x = a + b + b in two steps:
q = a + b  # gate 1
x = q + c  # gate 2

# backward pass:
dc = 1.0 * dx  # backprop gate 2
dq = 1.0 * dx
da = 1.0 * dq  # backprop gate 1
db = 1.0 * dq

x = a + b + c
da = 1.0 * dx
db = 1.0 * dx
dc = 1.0 * dx

# Okay, how about combining gates?:
x = a * b + c
# given dx, backprop in-one-sweep would be =>
da = b * dx
db = a * dx
dc = 1.0 * dx

# lets do our neuron in two steps:
q = a * x + b * y + c
f = math.sigmoid(q)  # sig is the sigmoid function
# and now backward pass, we are given df, and:
df = 1
dq = (f * (1 - f)) * df
# and now we chain it to the inputs
da = x * dq
dx = a * dq
dy = b * dq
db = y * dq
dc = 1.0 * dq

x = a * a
da = a * dx  # gradient into a from first branch
da += a * dx  # and add on the gradient from the second branch

# short form instead is:
da = 2 * a * dx

x = a * a + b * b + c * c
# we get:
da = 2 * a * dx
db = 2 * b * dx
dc = 2 * c * dx

x = math.pow(((a * b + c) * d), 2)

x1 = a * b + c
x2 = x1 * d
x = x2 * x2  # this is identical to the above expression for x
# and now in backprop we go backwards:
dx2 = 2 * x2 * dx  # backprop into x2
dd = x1 * dx2  # backprop into d
dx1 = d * dx2  # backprop into x1
da = b * dx1
db = a * dx1
dc = 1.0 * dx1  # done!
