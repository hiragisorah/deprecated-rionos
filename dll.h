#pragma once

template<class _Class> class DLLClass
{
public:
	DLLClass(void)
		: class_(nullptr)
		, dll_(nullptr)
	{}

private:
	_Class * class_;
	HMODULE dll_;

public:
	_Class * operator ->(void)
	{
		return this->class_;
	}

public:
	_Class * Class(void) { return this->class_; }
	template<class ... Args> bool Create(std::string path, Args ... args)
	{
		typedef _Class*(*CreateFunc)(Args ...);

		this->dll_ = LoadLibraryA(path.c_str());

		if (this->dll_ == nullptr)
		{
			std::cout << "DLLの読み込みに失敗しました" << std::endl;
			return false;
		}
		else
		{
			std::cout << "DLLの読み込みに成功しました" << std::endl;
		}

		CreateFunc Create = (CreateFunc)GetProcAddress(this->dll_, "Create");

		if (Create == nullptr)
		{
			std::cout << "関数のアドレス取得に失敗しました。" << std::endl;
			FreeLibrary(this->dll_);
			return false;
		}
		else
		{
			std::cout << "関数のアドレス取得に成功しました。" << std::endl;
		}

		this->class_ = Create(args ...);

		return true;
	}
	bool Destroy(void)
	{
		this->class_->Destroy();
		FreeLibrary(this->dll_);
	}
};

template<class _Class, class ... Args> _Class * CreateClassFromDLL(std::string path, Args ... args)
{
	typedef _Class*(*CreateFunc)(Args ...);

	auto dll = LoadLibraryA(path.c_str());

	if (dll == nullptr)
	{
		std::cout << "DLLの読み込みに失敗しました" << std::endl;
		return nullptr;
	}
	else
	{
		std::cout << "DLLの読み込みに成功しました" << std::endl;
	}

	CreateFunc Create = (CreateFunc)GetProcAddress(dll, "Create");

	if (Create == nullptr)
	{
		std::cout << "関数のアドレス取得に失敗しました。" << std::endl;
		FreeLibrary(dll);
		return nullptr;
	}
	else
	{
		std::cout << "関数のアドレス取得に成功しました。" << std::endl;
	}

	return Create(args ...);
}