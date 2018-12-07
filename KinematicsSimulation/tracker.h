#pragma once

template <typename Target, typename Save>
class tracker
{
	using traces_t = std::vector<std::shared_ptr<Save>>;
	using traces_p = std::shared_ptr<std::vector<std::shared_ptr<Save>>>;
public:
	tracker() = default;
	virtual  ~tracker() = default;
	bool has_target() const { return target_ == nullptr; }
	void bind(Target& obj) { target_ = &obj; }
	void clear() { history_->clear(); };
	virtual void trace() { history_->push_back(std::make_shared<Save>(*target_)); }

	traces_p history() const { return history_; }

private:
	Target* target_ = nullptr;
	traces_p history_ = std::make_shared<traces_t>();
};
