
#include "UMix.hpp"

void UMix::step() {
	if (!outputs[OUT_OUTPUT].active) {
		return;
	}
	if (params[MODE_PARAM].value > 0.5f) {
		float out = 0.0f;
		for (int i = 0; i < 6; ++i) {
			out += inputs[IN1_INPUT + i].value;
		}
		outputs[OUT_OUTPUT].value = _saturator.next(params[LEVEL_PARAM].value * out);
	}
	else {
		float out = 0.0f;
		int active = 0;
		for (int i = 0; i < 6; ++i) {
			if (inputs[IN1_INPUT + i].active) {
				out += inputs[IN1_INPUT + i].value;
				++active;
			}
		}
		if (active > 0) {
			out /= (float)active;
			outputs[OUT_OUTPUT].value = _saturator.next(params[LEVEL_PARAM].value * out);
		}
		else {
			outputs[OUT_OUTPUT].value = 0.0f;
		}
	}
}

struct UMixWidget : ModuleWidget {
	static constexpr int hp = 3;

	UMixWidget(UMix* module) : ModuleWidget(module) {
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);

		{
			SVGPanel *panel = new SVGPanel();
			panel->box.size = box.size;
			panel->setBackground(SVG::load(assetPlugin(plugin, "res/UMix.svg")));
			addChild(panel);
		}

		addChild(Widget::create<ScrewSilver>(Vec(0, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 15, 365)));

		// generated by svg_widgets.rb
		auto modeParamPosition = Vec(15.0, 255.5);
		auto levelParamPosition = Vec(14.5, 314.5);

		auto in1InputPosition = Vec(10.5, 23.0);
		auto in2InputPosition = Vec(10.5, 53.0);
		auto in3InputPosition = Vec(10.5, 83.0);
		auto in4InputPosition = Vec(10.5, 113.0);
		auto in5InputPosition = Vec(10.5, 143.0);
		auto in6InputPosition = Vec(10.5, 173.0);

		auto outOutputPosition = Vec(10.5, 203.0);
		// end generated by svg_widgets.rb

		addParam(ParamWidget::create<SliderSwitch2State14>(modeParamPosition, module, UMix::MODE_PARAM, 0.0, 1.0, 1.0));
		addParam(ParamWidget::create<Knob16>(levelParamPosition, module, UMix::LEVEL_PARAM, 0.0, 1.0, 1.0));

		addInput(Port::create<Port24>(in1InputPosition, Port::INPUT, module, UMix::IN1_INPUT));
		addInput(Port::create<Port24>(in2InputPosition, Port::INPUT, module, UMix::IN2_INPUT));
		addInput(Port::create<Port24>(in3InputPosition, Port::INPUT, module, UMix::IN3_INPUT));
		addInput(Port::create<Port24>(in4InputPosition, Port::INPUT, module, UMix::IN4_INPUT));
		addInput(Port::create<Port24>(in5InputPosition, Port::INPUT, module, UMix::IN5_INPUT));
		addInput(Port::create<Port24>(in6InputPosition, Port::INPUT, module, UMix::IN6_INPUT));

		addOutput(Port::create<Port24>(outOutputPosition, Port::OUTPUT, module, UMix::OUT_OUTPUT));
	}
};

RACK_PLUGIN_MODEL_INIT(Bogaudio, UMix) {
   Model* modelUMix = createModel<UMix, UMixWidget>("Bogaudio-UMix", "UMix", "unity mixer", MIXER_TAG);
   return modelUMix;
}
