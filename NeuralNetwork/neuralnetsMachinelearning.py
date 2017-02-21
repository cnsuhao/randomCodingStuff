# neuralnetsMachinelearning
import math
import random as rand


def forwardMultiplyGate(a, b):
    return a * b


def forwardAddGate(a, b):
    return a + b


def forwardCircuit(x,y,z):
    q = forwardAddGate(x, y)
    f = forwardMultiplyGate(q, z)
    return f


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


# A circuit: it takes 5 Units (x,y,a,b,c) and outputs a single Unit
# It can also compute the gradient w.r.t. its inputs
class Circuit():
	# create some gates
	mulg0 = multiplyGate()
	mulg1 = multiplyGate()
	addg0 = addGate()
	addg1 = addGate()

  	def forward(self, x, y, a, c, b):
	    self.ax = self.mulg0.forward(a, x)  # a*x
	    self.by = self.mulg1.forward(b, y)  # b*y
	    self.axpby = self.addg0.forward(self.ax, self.by)  # a*x + b*y
	    self.axpbypc = self.addg1.forward(self.axpby, c)  # a*x + b*y + c
	    return self.axpbypc

  	def backward(self, gradient_top):  # takes pull from above
	    self.axpbypc.grad = gradient_top
	    self.addg1.backward()  # sets gradient in axpby and c
	    self.addg0.backward()  # sets gradient in ax and by
	    self.mulg1.backward()  # sets gradient in b and y
	    self.mulg0.backward()  # sets gradient in a and x


# SVM class
class SVM():
	# random initial parameter values
	a = Unit(1.0, 0.0)
	b = Unit(-2.0, 0.0)
	c = Unit(-1.0, 0.0)

	circuit = Circuit()

	def forward(self, x, y):  # assume x and y are Units
		self.unit_out = self.circuit.forward(x, y, self.a, self.b, self.c)
		return self.unit_out

  	def backward(self, label):  # label is +1 or -1
	    # reset pulls on a,b,c
	    self.a.grad = 0.0
	    self.b.grad = 0.0
	    self.c.grad = 0.0

	    # compute the pull based on what the circuit output was
	    pull = 0.0
	    if(label is 1 and self.unit_out.value < 1):  # 'is' on sama kuin: '==='
	      	pull = 1  # the score was too low: pull up

	    if(label is -1 and self.unit_out.value > -1):
	      	pull = -1  # the score was too high for a positive example, pull down

	    self.circuit.backward(pull)  # writes gradient into x,y,a,b,c

	    # add regularization pull for parameters: towards zero and proportional to value
	    self.a.grad += -self.a.value
	    self.b.grad += -self.b.value

  	def learnFrom(self, x, y, label):
		self.forward(x, y)  # forward pass (set .value in all Units)
		self.backward(label)  # backward pass (set .grad in all Units)
		self.parameterUpdate()  # parameters respond to tug

  	def parameterUpdate(self):
	    step_size = 0.01
	    self.a.value += step_size * self.a.grad
	    self.b.value += step_size * self.b.grad
	    self.c.value += step_size * self.c.grad


# a function that computes the classification accuracy
def evalTrainingAccuracy():
	num_correct = 0
  	for i in range(0, len(data)):
	    x = Unit(data[i][0], 0.0)
	    y = Unit(data[i][1], 0.0)
	    true_label = labels[i]

	    # see if the prediction matches the provided label
	    if svm.forward(x, y).value > 0:
	    	predicted_label = 1
	    else:
	    	predicted_label = -1

	    if predicted_label is true_label:
	      	num_correct += 1

  	return num_correct / len(data)


# ##############################################

data = []
labels = []

data.append([1.2, 0.7])
labels.append(1)

data.append([-0.3, -0.5])
labels.append(-1)

data.append([3.0, 0.1])
labels.append(1)

data.append([-0.1, -1.0])
labels.append(-1)

data.append([-1.0, 1.1])
labels.append(-1)

data.append([2.1, -3])
labels.append(1)

svm = SVM()

# the learning loop
for iter in range(0, 400):
	# pick a random data point
	i = int(math.floor(rand.random() * len(data)))
	x = Unit(data[i][0], 0.0)
	y = Unit(data[i][1], 0.0)
	label = labels[i]
	svm.learnFrom(x, y, label)

	if iter % 25 == 0:  # every 10 iterations...
		print('training accuracy at iter ', iter, ' :', evalTrainingAccuracy())


# By the way, I intentionally structured the code in a modular way,
# but we could have trained an SVM with a much simpler code.
# Here is really what all of these classes and computations boil down to:
"""
a = 1, b = -2, c = -1 # initial parameters
for(iter = 0 iter < 400 iter++) {
  # pick a random data point
  i = Math.floor(Math.random() * data.length)
  x = data[i][0]
  y = data[i][1]
  label = labels[i]

  # compute pull
  score = a*x + b*y + c
  pull = 0.0
  if(label === 1 && score < 1) pull = 1
  if(label === -1 && score > -1) pull = -1

  # compute gradient and update parameters
  step_size = 0.01
  a += step_size * (x * pull - a) # -a is from the regularization
  b += step_size * (y * pull - b) # -b is from the regularization
  c += step_size * (1 * pull)
}
"""

# counting cost for each one

X = [[1.2, 0.7], [-0.3, 0.5], [3, 2.5]]  # array of 2-dimensional data
y = [1, -1, 1]  # array of labels
w = [0.1, 0.2, 0.3]  # example: random numbers
alpha = 0.1  # regularization strength


def cost(X, y, w):

	total_cost = 0.0  # L, in SVM loss function above
	N = len(X)
	for i in range(0, N):
		# loop over all data points and compute their score
		xi = X[i]
		score = w[0] * xi[0] + w[1] * xi[1] + w[2]

		# accumulate cost based on how compatible the score is with the label
		yi = y[i]  # label
		costi = max(0, - yi * score + 1)
		print 'example ', i, ': xi = (', xi, ') and label = ', yi
		print '  score computed to be %0.3f' % score
		print '  => cost computed to be %0.3f' % costi
		total_cost += costi

	# regularization cost: we want small weights
	reg_cost = alpha * (w[0] * w[0] + w[1] * w[1])
	print 'regularization cost for current model is %0.3f' % reg_cost
	total_cost += reg_cost

	print 'total cost is %0.3f' % total_cost
	return total_cost


print ""
print "counting costs:"
total_cost = cost(X, y, w)
