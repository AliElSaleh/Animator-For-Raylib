#pragma once
#include "raylib.h"

class Animator
{
public:
	Animator(const char* AnimatorName, unsigned int NumOfFramesPerRow, unsigned int NumOfRows, unsigned int Speed, bool bPlayInReverse = false, bool bContinuous = false, bool bLooping = true);
	~Animator();

	void AssignSprite(const Texture2D& Sprite);
	void ChangeSprite(const Texture2D& NewSprite, unsigned int NumOfFramesPerRow, unsigned int NumOfRows, unsigned int Speed, float DelayInSeconds = 0.0f, bool bContinuous = false, bool bLooping = true);
	void FlipSprite(bool bHorizontalFlip, bool bVerticalFlip = true);
	
	void SetLooping(bool bLooping);
	void SetContinuous(bool bIsContinuous);
	void SetFramerate(unsigned int NewFramerate);

	void GoToFrame(unsigned int FrameNumber);
	void GoToRow(unsigned int RowNumber);
	void GoToColumn(unsigned int ColumnNumber);
	void GoToFirstRow();
	void GoToFirstColumn();
	void GoToLastRow();
	void GoToLastColumn();
	void GoToFirstFrame();
	void GoToLastFrame();

	void NextRow();
	void PreviousRow();
	void NextColumn();
	void PreviousColumn();

	void Play();
	void Start();
	void Stop();
	void Pause(bool bToggle = false);
	void UnPause();
	void Forward();
	void Reverse(bool bToggle = false);
	void Restart();

	unsigned int GetTotalFrames() const;
	unsigned int GetTotalRows() const;
	unsigned int GetTotalColumns() const;
	unsigned int GetCurrentFrame() const;
	unsigned int GetCurrentRow() const;
	unsigned int GetCurrentColumn() const;
	unsigned int GetTotalTimeInFrames() const;
	unsigned int GetTimeRemainingInFrames() const;
	float GetTotalTimeInSeconds() const;
	float GetTimeRemainingInSeconds() const;
	const char* GetName() const;

	bool IsAtFrame(unsigned int FrameNumber) const;
	bool IsAtRow(unsigned int RowNumber) const;
	bool IsAtColumn(unsigned int ColumnNumber) const;
	bool IsAtFirstFrame();
	bool IsAtLastFrame();
	bool IsAtFirstRow() const;
	bool IsAtFirstColumn() const;
	bool IsAtLastRow() const;
	bool IsAtLastColumn() const;
	bool IsPlaying() const;
	bool IsStartedPlaying();
	bool IsFinishedPlaying();

	Rectangle GetFrameRec() const;
	Texture2D GetSprite() const;

private:
	void CountdownInFrames();
	void ResetTimer();
	void ResetFrameRec();

	void NextFrame();
	void PreviousFrame();

	float FrameWidth{};
	float FrameHeight{};
	float TimeRemainingFramesCounter{};

	unsigned int PlaybackPosition;
	unsigned int DelayFramesCounter;

	unsigned int Rows;
	unsigned int Columns;
	unsigned int Framerate;
	unsigned int CurrentRow;
	unsigned int CurrentColumn;
	unsigned int CurrentFrame;

	bool bFlipH{};
	bool bFlipV{};
	bool bCanLoop;
	bool bReverse{};
	bool bContinuous{};
	bool bPaused{};
	bool bIsAnimationFinished{};

	Rectangle FrameRec{};
	Texture2D Sprite{};
	const char* Name{};
};
