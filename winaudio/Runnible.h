#pragma once

class Runnible
{
public:
	Runnible(void);
	~Runnible(void);

	void Enable();
	void Disable();
	bool IsEnabled();
protected:
	virtual void OnEnable() = 0;
	virtual void OnDisable() = 0;
private:
	bool m_Enabled;
};
