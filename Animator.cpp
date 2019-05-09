#include "Animator.h"
#include <cstdio>

Animator::Animator(const char* AnimatorName, const unsigned int NumOfFramesPerRow, const unsigned int NumOfRows, const unsigned int Speed, const bool bPlayInReverse, const bool bContinuous, const bool bLooping)
{
	Name = AnimatorName;
	Framerate = Speed == 0 ? 1 : Speed;
	Columns = NumOfFramesPerRow;
	Rows = NumOfRows == 0 ? 1 : NumOfRows;
	bReverse = bPlayInReverse;
	bCanLoop = bLooping;
	this->bContinuous = bContinuous;
	PlaybackPosition = 0;
	DelayFramesCounter = 0;
	CurrentFrame = 0;
	CurrentRow = 0;
	CurrentColumn = 0;
	bFlipH = false;
	bFlipV = false;
}

void Animator::AssignSprite(const Texture2D& Sprite)
{
	this->Sprite = Sprite;

	Restart();
}

void Animator::ChangeSprite(const Texture2D& NewSprite, const unsigned int NumOfFramesPerRow, const unsigned int NumOfRows, const unsigned int Speed, const float DelayInSeconds, const bool bPlayInReverse, const bool bContinuous, const bool bLooping)
{
	DelayFramesCounter++;

	if (GetFPS() >= 0)
	{
		if (DelayFramesCounter > DelayInSeconds * GetFPS())
		{
			Rows = NumOfRows == 0 ? 1 : NumOfRows;
			Columns = NumOfFramesPerRow;
			Framerate = Speed;
			bCanLoop = bLooping;
			this->bContinuous = bContinuous;
			bReverse = bPlayInReverse;
			PlaybackPosition = 0;
			DelayFramesCounter = 0;
			bIsAnimationFinished = false;
			bHasStartedPlaying = !bPaused;

			AssignSprite(NewSprite);
		}
	}
}

void Animator::FlipSprite(const bool bHorizontalFlip, const bool bVerticalFlip)
{
	//FrameRec.width = fabsf(FrameRec.width) * (bHorizontalFlip ? -1 : 1);
	//FrameRec.height = fabsf(FrameRec.height) * (bVerticalFlip ? -1 : 1);

	bFlipH = bHorizontalFlip;
	bFlipV = !bFlipV;

	if (bHorizontalFlip && bVerticalFlip)
	{
		FrameRec.width *= -1;
		FrameRec.height *= -1;
	}
	else if (bHorizontalFlip)
	{
		FrameRec.width *= -1;
	}
	else if (bVerticalFlip)
	{	
		FrameRec.height *= -1;
	}

	printf("Width: %f, Height: %f\n", FrameRec.width, FrameRec.height);
}

void Animator::SetLooping(const bool bLooping)
{
	bCanLoop = bLooping;
}

void Animator::SetContinuous(const bool bIsContinuous)
{
	bContinuous = bIsContinuous;
}

void Animator::ResetFrameRec()
{
	FrameRec.width = bFlipH ? -float(Sprite.width) / Columns : float(Sprite.width) / Columns;
	FrameRec.height = bFlipV ? -float(Sprite.height) / Rows : float(Sprite.height) / Rows;
	FrameWidth =  FrameRec.width;
	FrameHeight = FrameRec.height;
	FrameRec.x = bReverse && bContinuous ? Sprite.width - FrameWidth : 0;
	FrameRec.y = bReverse && bContinuous ? Sprite.height - FrameHeight : 0;

	CurrentFrame = bReverse ? Columns - 1 : 0;
	CurrentRow = bReverse ? Rows - 1 : 0;
	CurrentColumn = bReverse ? Columns - 1 : 0;
}

void Animator::GoToRow(const unsigned int RowNumber)
{
	if (RowNumber >= Rows)
	{
		FrameRec.y = (Rows - 1) * FrameHeight;
		CurrentRow = Rows - 1;
		TimeRemainingFramesCounter = float(GetTotalTimeInFrames());
	}
	else if (Rows >= 1)
	{
		FrameRec.y = RowNumber == 0 ? 0 : RowNumber * FrameHeight;
		CurrentRow = RowNumber;
		TimeRemainingFramesCounter = float(GetTotalTimeInFrames() - (RowNumber*Columns + Columns));
	}
}

void Animator::GoToColumn(const unsigned int ColumnNumber)
{
	if (ColumnNumber >= Columns)
	{
		FrameRec.x = (Columns - 1) * FrameWidth;
		CurrentColumn = Columns - 1;
		CurrentFrame = Columns - 1;
		TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - CurrentRow*Columns;
	}
	else if (Columns >= 1)
	{
		FrameRec.x = ColumnNumber == 0 ? 0 : ColumnNumber * FrameWidth;
		CurrentColumn = ColumnNumber;
		CurrentFrame = ColumnNumber;
		TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - CurrentRow*Columns - ColumnNumber;
	}
}

void Animator::GoToFirstRow()
{
	GoToRow(0);

	// Update time remaining
	TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - CurrentColumn;
}

void Animator::GoToFirstColumn()
{
	if (!bIsAnimationFinished)
	{
		GoToColumn(0);

		// Update time remaining
		bContinuous ? TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - Columns*CurrentRow : TimeRemainingFramesCounter = float(Columns);
	}
	else
	{
		GoToColumn(0);
		TimeRemainingFramesCounter = bContinuous ? float(GetTotalTimeInFrames()) - float(GetTotalTimeInFrames()) / Rows * CurrentRow : 0;
	}
}

void Animator::GoToLastRow()
{
	GoToRow(Rows - 1);

	// Update time remaining
	bContinuous ? TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - CurrentColumn - Columns*(Rows-1) : TimeRemainingFramesCounter = float(Columns - CurrentColumn);
}

void Animator::GoToLastColumn()
{
	if (!bIsAnimationFinished)
	{
		GoToColumn(Columns - 1);

		// Update time remaining
		if (bContinuous)
		{
			if (!bReverse)
			{
				if (Columns*CurrentRow != 0)
					TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - Columns*CurrentRow + Columns;
				else
					TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - Columns;
			}
			else
			{
				TimeRemainingFramesCounter = float(Columns*CurrentRow + Columns);
			}

		}
		else
			TimeRemainingFramesCounter = bIsAnimationFinished ? 0.0f : float(Columns);
	}
	else
	{
		GoToColumn(Columns - 1);
		TimeRemainingFramesCounter = float(GetTotalTimeInFrames()) - CurrentRow * Columns - Columns;
	}
}

void Animator::GoToFrame(const unsigned int FrameNumber)
{
	// Does frame exist in sprite-sheet
	if (FrameNumber < Columns * Rows)
	{
		GoToRow(FrameNumber / Columns);
		GoToColumn(FrameNumber % Columns);
	}
	else
		printf("ERROR from GoToFrame(): Frame %u does not exist! %s sprite has frames from %u to %u.\n", FrameNumber, Name, 0, Rows*Columns-1);
}

void Animator::GoToFirstFrame()
{
	GoToColumn(0);
}

void Animator::GoToLastFrame()
{
	GoToColumn(Columns - 1);
}

void Animator::GoToFirstFrameOfSpriteSheet()
{
	GoToRow(0);
	GoToColumn(0);
}

void Animator::GoToLastFrameOfSpriteSheet()
{
	GoToRow(Rows - 1);
	GoToColumn(Columns - 1);
}

void Animator::NextFrame()
{
	// Only increment when animation is playing
	if (!bIsAnimationFinished)
	{
		CurrentFrame++;
		CurrentColumn++;
	}

	if (bCanLoop)
	{
		// Are we over the max columns
		if (CurrentFrame > Columns - 1)
		{
			// If we are continuous, Go to the next row in the sprite-sheet
			if (bContinuous)
			{
				NextRow();
				GoToFirstColumn();
			}
			// Otherwise, Go back to the start
			else
			{
				GoToFirstColumn();
			}
		}
	}
	else
	{
		// Are we over the max columns
		if (CurrentFrame > Columns - 1)
		{
			// If we are continuous, Go to the next row in the sprite-sheet
			if (bContinuous)
			{
				// Clamp values back down
				CurrentFrame = Columns - 1;
				CurrentColumn = Columns - 1;

				// Go to next row if we are not at the last frame
				if (!IsAtLastFrame())
				{
					NextRow();
					GoToFirstColumn();
				}
				else
					bIsAnimationFinished = true;
				
			}
			// Otherwise, Stay at the end
			else
			{
				bIsAnimationFinished = true;
				GoToLastColumn();
			}
		}
	}
}

void Animator::PreviousFrame()
{
	// Only decrement when animation is playing
	if (!bIsAnimationFinished)
	{
		CurrentFrame--;
		CurrentColumn--;
	}

	if (bCanLoop)
	{
		// Are we over the max columns OR equal to zero
		if (CurrentFrame == 0 || CurrentFrame > Columns)
		{
			// If we are continuous, Go to the previous row in the sprite-sheet
			if (bContinuous)
			{
				PreviousRow();
				GoToLastColumn();
			}
			// Otherwise, Go back to the last column
			else
			{
				GoToLastColumn();
			}
		}
	}
	else
	{
		// Are we over the max columns OR equal to zero
		if (CurrentFrame == 0 || CurrentFrame > Columns)
		{
			// If we are continuous, Go to the previous row in the sprite-sheet
			if (bContinuous)
			{
				// Clamp values back down
				CurrentFrame = 0;
				CurrentColumn = 0;

				// Go to previous row if we are not at the first frame
				if (!IsAtFirstFrame())
				{
					PreviousRow();
					GoToLastColumn();
				}
				else
					bIsAnimationFinished = true;
			}
			// Otherwise, Stay at the start
			else
			{
				bIsAnimationFinished = true;
				GoToFirstColumn();
			}
		}
	}
}

float Animator::Lerp(const float Start, const float End, const float Alpha)
{
	return (1.0f - Alpha) * Start + Alpha * End;
}

void Animator::NextRow()
{
	FrameRec.y += FrameHeight;

	if (FrameRec.y >= Sprite.height)
	{
		// Go to start
		if (bCanLoop)
		{
			FrameRec.y = 0;
			CurrentRow = 0;
		}
		// Stay at end
		else
		{
			FrameRec.y = float(Sprite.height);
			CurrentRow = Rows - 1;
		}

		ResetTimer();
	}
	else
		CurrentRow++;

	// Update the time remaining
	TimeRemainingFramesCounter = GetTotalTimeInFrames() - (float(GetTotalTimeInFrames()) / Rows) * CurrentRow;
}

void Animator::PreviousRow()
{
	FrameRec.y -= FrameHeight;

	if (FrameRec.y < 0)
	{
		FrameRec.y = float(Sprite.height) - FrameHeight;
		CurrentRow = Rows - 1;
		ResetTimer();
	}
	else
		CurrentRow--;

	// Update the time remaining
	if (!bReverse)
		TimeRemainingFramesCounter = GetTotalTimeInFrames() - (float(GetTotalTimeInFrames()) / Rows) * CurrentRow;
}

void Animator::NextColumn()
{
	FrameRec.x += FrameWidth;

	if (FrameRec.x > Sprite.width)
	{
		FrameRec.x = 0;
		CurrentColumn = 0;
	}
	else
		CurrentColumn++;

	// Update the time remaining
	TimeRemainingFramesCounter -= 1;
}

void Animator::PreviousColumn()
{
	FrameRec.x -= FrameWidth;

	if (FrameRec.x < 0)
	{
		FrameRec.x = float(Sprite.width) - FrameWidth;
		CurrentColumn = Columns - 1;
	}
	else
		CurrentColumn--;

	// Update the time remaining
	TimeRemainingFramesCounter += 1;
}

void Animator::Forward()
{
	if (bReverse)
		bReverse = false;
}

void Animator::Reverse(const bool bToggle)
{
	if (bToggle)
	{
		bReverse = !bReverse;
		TimeRemainingFramesCounter += GetTotalTimeInFrames() - TimeRemainingFramesCounter*2;
		bIsAnimationFinished = false;
	}
	else
	{
		bReverse = true;
		TimeRemainingFramesCounter += GetTotalTimeInFrames() - TimeRemainingFramesCounter*2;
		bIsAnimationFinished = false;
	}
}

void Animator::Restart()
{
	ResetFrameRec();
	ResetTimer();
	bHasStartedPlaying = true;
}

unsigned Animator::GetTotalFrames() const
{
	return Rows * Columns;
}

unsigned Animator::GetTotalRows() const
{
	return Rows;
}

unsigned Animator::GetTotalColumns() const
{
	return Columns;
}

unsigned Animator::GetCurrentFrame() const
{
	return CurrentRow*Columns + CurrentColumn;
}

unsigned Animator::GetCurrentRow() const
{
	return CurrentRow;
}

unsigned Animator::GetCurrentColumn() const
{
	return  CurrentColumn;
}

unsigned int Animator::GetTotalTimeInFrames() const
{
	return bContinuous ? Columns * Rows : Columns;
}

float Animator::GetTotalTimeInSeconds() const
{
	return bContinuous ? float(Columns * Rows) / Framerate : float(Columns) / Framerate;
}

unsigned Animator::GetTimeRemainingInFrames() const
{
	return unsigned(TimeRemainingFramesCounter);
}

float Animator::GetTimeRemainingInSeconds() const
{
	return float(TimeRemainingFramesCounter) / Framerate;
}

const char* Animator::GetName() const
{
	return Name;
}

void Animator::CountdownInFrames()
{
	if (TimeRemainingFramesCounter != 0.0f)
		TimeRemainingFramesCounter -= GetFrameTime() < 0.01f ? Framerate * GetFrameTime() : 0.0f;

	if (TimeRemainingFramesCounter <= 0.0f)
		TimeRemainingFramesCounter = 0.0f;
}

void Animator::Play()
{
	if (!bPaused)
	{
		PlaybackPosition++;

		// Update the time remaining
		if (!bIsAnimationFinished)
			CountdownInFrames();

		// Has 'X' amount of frames passed?
		if (PlaybackPosition > GetFPS() / Framerate)
		{
			// Reset playback position
			PlaybackPosition = 0;

			// Go to previous frame when reversing
			if (bReverse)
				PreviousFrame();
			// Go to next frame if not reversing
			else
				NextFrame();
		}

		// Only go to next frame if animation has not finished playing
		if (!bIsAnimationFinished)
			FrameRec.x = float(CurrentFrame)*FrameWidth;

		//printf("Row: %u, Column: %u\n", CurrentRow, CurrentColumn);
		bHasStartedPlaying = false;
	}
}

void Animator::LerpAnim(const float Speed, const bool bConstant)
{
	PlaybackPosition++;
	if (PlaybackPosition > GetFPS() / Framerate)
	{
		PlaybackPosition = 0;

		if (bConstant)
			FrameRec.x += Speed * GetFrameTime(); 
		else
			FrameRec.x = Lerp(FrameRec.x, Sprite.width, Speed * GetFrameTime());
	}
}

void Animator::Start()
{
	UnPause();

	if (!bHasStartedPlaying)
		bHasStartedPlaying = true;
}

void Animator::Stop()
{
	PlaybackPosition = 0;
	CurrentColumn = 0;
	CurrentFrame = 0;
	CurrentRow = 0;
	bHasStartedPlaying = true;
	bIsAnimationFinished = true;

	ResetFrameRec();
	ResetTimer();
	Pause();
}

void Animator::UnPause()
{
	bPaused = false;
	bHasStartedPlaying = true;
}

void Animator::Pause(const bool bToggle)
{
	if (bToggle)
	{
		bPaused = !bPaused;
		bHasStartedPlaying = !bPaused;
	}
	else
	{
		bPaused = true;
		bHasStartedPlaying = false;
	}
}

void Animator::SetFramerate(const unsigned int NewFramerate)
{
	Framerate = NewFramerate;
}

bool Animator::IsAtFrame(const unsigned int FrameNumber) const
{
	// Does frame exist in sprite-sheet
	if (FrameNumber < Columns * Rows)
	{
		const unsigned int RowFrameNumberIsOn = FrameNumber / Columns;
		const unsigned int ColumnFrameNumberIsOn = FrameNumber % Columns;

		return IsAtRow(RowFrameNumberIsOn) && IsAtColumn(ColumnFrameNumberIsOn);
	}

	printf("ERROR from IsAtFrame(): Frame %u does not exist! %s sprite has frames from %u to %u.\n", FrameNumber, Name, 0, Rows*Columns-1);
	return false;
}

bool Animator::IsAtRow(const unsigned int RowNumber) const
{
	if (RowNumber < Rows)
		return RowNumber == CurrentRow;

	printf("ERROR from IsAtRow(): Row does not exist!\n");
	return false;
}

bool Animator::IsAtColumn(const unsigned int ColumnNumber) const
{
	if (ColumnNumber < Columns)
		return ColumnNumber == CurrentColumn;

	printf("ERROR from IsAtColumn(): Column does not exist!\n");
	return false;
}

bool Animator::IsAtFirstFrameOfSpriteSheet() const
{
	return IsAtFirstRow() && IsAtFirstColumn();
}

bool Animator::IsAtLastFrameOfSpriteSheet() const
{
	return IsAtLastRow() && IsAtLastColumn();
}

bool Animator::IsAtFirstRow() const
{
	return CurrentRow == 0;
}

bool Animator::IsAtFirstColumn() const
{
	return CurrentColumn == 0;
}

bool Animator::IsAtFirstFrame() const
{
	return bContinuous ? IsAtFirstRow() && IsAtFirstColumn() : IsAtFirstColumn();
}

bool Animator::IsAtLastFrame() const
{
	return bContinuous ? IsAtLastRow() && IsAtLastColumn() : IsAtLastColumn();
}

void Animator::ResetTimer()
{
	TimeRemainingFramesCounter = float(GetTotalTimeInFrames());
}

Rectangle Animator::GetFrameRec() const
{
	return FrameRec;
}

Texture2D Animator::GetSprite() const
{
	return Sprite;
}

bool Animator::IsAtLastRow() const
{
	return CurrentRow == Rows - 1;
}

bool Animator::IsAtLastColumn() const
{
	return CurrentColumn == Columns - 1;
}

bool Animator::IsStartedPlaying()
{
	if (IsAtFirstFrame())
	{
		ResetTimer();
		return true;
	}

	return bHasStartedPlaying;
}

bool Animator::IsFinishedPlaying()
{
	if (IsAtLastFrame())
	{
		ResetTimer();
		return true;
	}

	if (!bCanLoop)
		return bIsAnimationFinished;

	return bIsAnimationFinished;
}

bool Animator::IsPlaying() const
{
	if (bCanLoop)
		return !bPaused;
	
	if (!bCanLoop && bContinuous)
		return !bIsAnimationFinished;

	return !bIsAnimationFinished;
}
