class Test:
    def fire(self, inputs):
        outputs = {}
        for key in inputs:
            outputs[key] = inputs[key] * 2
        return outputs
