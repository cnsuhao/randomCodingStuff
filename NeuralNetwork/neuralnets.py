#!/usr/bin/env python

# neuralnets http: #karpathy.github.io/neuralnets/
import random as rand
import math


def forwardMultiplyGate(a, b):
    return a * b


def forwardAddGate(a, b):
    return a + b


def forwardCircuit(x,y,z):
    q = forwardAddGate(x, y)
    f = forwardMultiplyGate(q, z)
    return f


# Strategy 1: Random local search
def RandomLocalSearch(inX, inY, rounds):

    # circuit with single gate for now
    x = inX
    y = inY  # some input values

    # try changing x,y randomly small amounts and keep track of what works best
    tweak_amount = 0.01
    best_out = -float("inf")
    best_x = x
    best_y = y

    for k in range(0,rounds + 1):
        x_try = x + tweak_amount * (rand.random() * 2 - 1)  # tweak x a bit
        y_try = y + tweak_amount * (rand.random() * 2 - 1)  # tweak y a bit
        out = forwardMultiplyGate(x_try, y_try)
        if out > best_out:
            # best improvement yet! Keep track of the x and y
            best_out = out
            best_x = x_try
            best_y = y_try

    return best_out, best_x, best_y


# Strategy 2: numerial gradient
def numericalGradient(inX, inY):
    x = inX
    y = inY
    out = forwardMultiplyGate(x, y)  # -6
    h = 0.0001

    # compute derivative with respect to x
    xph = x + h  # -1.9999
    out2 = forwardMultiplyGate(xph, y)  # -5.9997
    x_derivative = (out2 - out) / h  # 3.0

    # compute derivative with respect to y
    yph = y + h  # 3.0001
    out3 = forwardMultiplyGate(x, yph)  # -6.0002
    y_derivative = (out3 - out) / h  # -2.0

    # gradient
    step_size = 0.01
    out = forwardMultiplyGate(x, y)  # before: -6
    x = x + step_size * x_derivative  # x becomes -1.97
    y = y + step_size * y_derivative  # y becomes 2.98
    out_new = forwardMultiplyGate(x, y)  # -5.87! exciting

    return out_new


# Strategy 3: analytic gradient
def AnalyticGradient(inX, inY):
    x = inX
    y = inY
    # out = forwardMultiplyGate(x, y)  # before: -6
    x_gradient = y  # by our complex mathematical derivation above
    y_gradient = x

    step_size = 0.01
    x += step_size * x_gradient  # -2.03
    y += step_size * y_gradient  # 2.98
    out_new = forwardMultiplyGate(x, y)  # -5.87. Higher output! Nice.

    return out_new


# Recursive case: Circuits with Multiple Gates
def Recursive(inX, inY, inZ):
    x = inX
    y = inY
    z = inZ
    f = forwardCircuit(x, y, z)  # output is -12
    return f


# BackPropagation
def backpropagation(inX, inY, inZ):
    # initial conditions
    x = inX
    y = inY
    z = inZ

    q = forwardAddGate(x, y)  # q is 3
    f = forwardMultiplyGate(q, z)  # output is -12

    # gradient of the MULTIPLY gate with respect to its inputs
    # wrt is short for "with respect to"
    derivative_f_wrt_z = q  # 3
    derivative_f_wrt_q = z  # -4

    # derivative of the ADD gate with respect to its inputs
    derivative_q_wrt_x = 1.0
    derivative_q_wrt_y = 1.0

    # chain rule
    derivative_f_wrt_x = derivative_q_wrt_x * derivative_f_wrt_q  # -4
    derivative_f_wrt_y = derivative_q_wrt_y * derivative_f_wrt_q  # -4

    # final gradient, from above: [-4, -4, 3]
    gradient_f_wrt_xyz = [derivative_f_wrt_x, derivative_f_wrt_y, derivative_f_wrt_z]

    # let the inputs respond to the force/tug:
    step_size = 0.01
    x = x + step_size * derivative_f_wrt_x  # -2.04
    y = y + step_size * derivative_f_wrt_y  # 4.96
    z = z + step_size * derivative_f_wrt_z  # -3.97

    # Our circuit now better give higher output:
    q = forwardAddGate(x, y)  # q becomes 2.92
    f = forwardMultiplyGate(q, z)  # output is -11.59, up from -12! Nice!

    return f, gradient_f_wrt_xyz


# numerical gradient check
def NumericalGradientCheck(inX, inY, inZ):
    # initial conditions
    x = inX
    y = inY
    z = inZ

    h = 0.0001
    x_derivative = (forwardCircuit(x + h, y, z) - forwardCircuit(x, y, z)) / h  # -4
    y_derivative = (forwardCircuit(x, y + h, z) - forwardCircuit(x, y, z)) / h  # -4
    z_derivative = (forwardCircuit(x, y, z + h) - forwardCircuit(x, y, z)) / h  # 3

    return [x_derivative, y_derivative, z_derivative]


#  Single Neuron


# #########################

x = -2
y = 3
z = -4

"""
rounds = 100
best_out, best_x, best_y = RandomLocalSearch(x, y, rounds)
print "x:", x, " y:", y
print "best_out", best_out, " best_x:", best_x, " best_y:", best_y
print ""
"""

"""
out_new = numericalGradient(x, y)
print "x:", x, " y:", y
print "out_new:", out_new
print ""

out_new2 = AnalyticGradient(x, y)
print "x:", x, " y:", y
print "out_new2:", out_new2
print ""

y = 5

f = Recursive(x, y, z)
print "x:", x, " y:", y, " z:", z
print "f:", f
print ""

f2, gradient_f_wrt_xyz = backpropagation(x, y, z)
print "x:", x, " y:", y, " z:", z
print "f2:", f2, " gradient_f_wrt_xyz:", gradient_f_wrt_xyz
print ""

derivative_xyz = NumericalGradientCheck(x, y, z)
print "x:", x, " y:", y, " z:", z
print "derivative_xyz:", derivative_xyz
print ""
"""


# every Unit corresponds to a wire in the diagrams
class Unit:
    def __init__(self, value, grad):
        # value computed in the forward pass
        self.value = value
        # the derivative of circuit output w.r.t this unit, computed in backward pass
        self.grad = grad


class multiplyGate():
    def forward(self, u0, u1):
        # store pointers to input Units u0 and u1 and output unit utop
        self.u0 = u0
        self.u1 = u1
        self.utop = Unit(u0.value * u1.value, 0.0)
        return self.utop

    def backward(self):
        # take the gradient in output unit and chain it with the
        # local gradients, which we derived for multiply gate before
        # then write those gradients to those Units.
        self.u0.grad += self.u1.value * self.utop.grad
        self.u1.grad += self.u0.value * self.utop.grad


class addGate():
    def forward(self, u0, u1):
        self.u0 = u0
        self.u1 = u1  # store pointers to input units
        self.utop = Unit(u0.value + u1.value, 0.0)
        return self.utop

    def backward(self):
        # add gate. derivative wrt both inputs is 1
        self.u0.grad += 1 * self.utop.grad
        self.u1.grad += 1 * self.utop.grad


class sigmoidGate():
    # helper function
    def sig(self, x):
        return 1 / (1 + math.exp(-x))

    def forward(self, u0):
        self.u0 = u0
        self.utop = Unit(self.sig(self.u0.value), 0.0)
        return self.utop

    def backward(self):
        s = self.sig(self.u0.value)
        self.u0.grad += (s * (1 - s)) * self.utop.grad


# do the forward pass
def forwardNeuron(a, b, c, x, y):
    ax = mulg0.forward(a, x)  # a*x = -1
    by = mulg1.forward(b, y)  # b*y = 6
    axpby = addg0.forward(ax, by)  # a*x + b*y = 5
    axpbypc = addg1.forward(axpby, c)  # a*x + b*y + c = 2
    s = sg0.forward(axpbypc)  # sig(a*x + b*y + c) = 0.8808
    return s


def forwardCircuitFast(a,b,c,x,y):
    return 1 / (1 + math.exp(- (a * x + b * y + c)))


# create input units
a = Unit(1.0, 0.0)
b = Unit(2.0, 0.0)
c = Unit(-3.0, 0.0)
x = Unit(-1.0, 0.0)
y = Unit(3.0, 0.0)

# create the gates
mulg0 = multiplyGate()
mulg1 = multiplyGate()
addg0 = addGate()
addg1 = addGate()
sg0 = sigmoidGate()

s = forwardNeuron(a, b, c, x, y)

print('circuit output: ', s.value)  # prints 0.8808

s.grad = 1.0
sg0.backward()  # writes gradient into axpbypc
addg1.backward()  # writes gradients into axpby and c
addg0.backward()  # writes gradients into ax and by
mulg1.backward()  # writes gradients into b and y
mulg0.backward()  # writes gradients into a and x

step_size = 0.01
a.value += step_size * a.grad  # a.grad is -0.105
b.value += step_size * b.grad  # b.grad is 0.315
c.value += step_size * c.grad  # c.grad is 0.105
x.value += step_size * x.grad  # x.grad is 0.105
y.value += step_size * y.grad  # y.grad is 0.210

s = forwardNeuron(a, b, c, x, y)
print('circuit output after one backprop: ', s.value)  # prints 0.8825

# Success! 0.8825 is higher than the previous value, 0.8808.
# Finally, lets verify that we implemented the backpropagation
# correctly by checking the numerical gradient:
a = 1
b = 2
c = -3
x = -1
y = 3

h = 0.0001

a_grad = (forwardCircuitFast(a + h,b,c,x,y) - forwardCircuitFast(a,b,c,x,y)) / h
b_grad = (forwardCircuitFast(a,b + h,c,x,y) - forwardCircuitFast(a,b,c,x,y)) / h
c_grad = (forwardCircuitFast(a,b,c + h,x,y) - forwardCircuitFast(a,b,c,x,y)) / h
x_grad = (forwardCircuitFast(a,b,c,x + h,y) - forwardCircuitFast(a,b,c,x,y)) / h
y_grad = (forwardCircuitFast(a,b,c,x,y + h) - forwardCircuitFast(a,b,c,x,y)) / h

gradients = [a_grad, b_grad, c_grad, x_grad, y_grad]

print "gradients:", gradients
