#include <wx/app.h>
#include <wx/frame.h>
#include <wx/bitmap.h>
#include <wx/menu.h>
#include <wx/dcclient.h>
#include <wx/bmpbuttn.h>
#include <wx/panel.h>

#include <SineWave.h>
#include <RtAudio.h>

enum
{
  wxID_PLAY = 1,
  wxID_BUTTONPLAY,
};

class MyApp : public wxApp
{
public:
  virtual bool OnInit() override;
};

class MainFrame : public wxFrame
{
public:
  MainFrame();

  void OnExit(wxCommandEvent& event);
  void OnPlayButtonClicked(wxCommandEvent& event);
  void OnPaint(wxPaintEvent& evt);
private:
  void OnPlay();
  void OnPause();
  wxBitmap m_bmp;
  wxPoint m_delta;

  RtAudio* audioStream;
  stk::SineWave sine;
  RtAudio::StreamParameters params;
  unsigned int bufferFrames = stk::RT_BUFFER_SIZE;
  double frequency = 440.0;

  bool isPlaying = false;
  wxDECLARE_EVENT_TABLE();
};

int tick(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* dataPointer)
{
  stk::SineWave* sine = (stk::SineWave*) dataPointer;
  stk::StkFloat* samples = (stk::StkFloat*) outputBuffer;

  for (unsigned int i=0; i<nBufferFrames; i++)
    *samples++ = sine->tick();

  return 0;
}

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
  if (!wxApp::OnInit())
    return false;
  wxInitAllImageHandlers();
  new MainFrame;
  return true;
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MainFrame::OnExit)
  EVT_PAINT(MainFrame::OnPaint)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200,100), 0 | wxFRAME_NO_TASKBAR | wxFRAME_SHAPED | wxSTAY_ON_TOP | wxSIMPLE_BORDER)
{
  m_bmp = wxBitmap("round_rect.png", wxBITMAP_TYPE_PNG);
  SetSize(wxSize(m_bmp.GetWidth(), m_bmp.GetHeight()));
  SetToolTip("Right-click to close, double click to cycle shape");
  SetShape(wxRegion(m_bmp, *wxWHITE));

  wxPanel *panel = new wxPanel(this, -1, wxPoint(0, 100), wxSize(320, 100));
  wxBitmap bitmap;
  bitmap.LoadFile("chiave_di_sol.bmp", wxBITMAP_TYPE_BMP);

  // The last parameter (0) - so we have 2D behaviour
  wxPoint buttonPosition = wxDefaultPosition + wxPoint(0.5*this->GetSize().GetWidth(),0.5*this->GetSize().GetHeight());
  wxBitmapButton *button1 = new wxBitmapButton(panel, wxID_BUTTONPLAY, bitmap, buttonPosition, bitmap.GetSize(), 0);
  Bind(wxEVT_BUTTON, &MainFrame::OnPlayButtonClicked, this, wxID_BUTTONPLAY);

  stk::Stk::setSampleRate(44100.0);
  audioStream = new RtAudio;
  params.deviceId = audioStream->getDefaultOutputDevice();
  params.nChannels = 1;
  RtAudioFormat format = ( sizeof(stk::StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

  if (audioStream->openStream(&params, NULL, format, (unsigned int) stk::Stk::sampleRate(), &bufferFrames, &tick, (void *) &sine ))
  {
    std::cout << audioStream->getErrorText() << std::endl;
  }

  Show();
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MainFrame::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
  wxPaintDC dc(this);
  dc.DrawBitmap(m_bmp, 0, 0, true);
}

void MainFrame::OnPlayButtonClicked(wxCommandEvent& WXUNUSED(event))
{
  if (!isPlaying)
    OnPlay();
  else
    OnPause();
}

void MainFrame::OnPlay()
{
  sine.setFrequency(frequency);
  if (audioStream->startStream())
  {
    std::cout << audioStream->getErrorText() << std::endl;
  }
  isPlaying = true;
}

void MainFrame::OnPause()
{
  audioStream->stopStream();
  // delete audioStream;
  isPlaying = false;
  frequency = frequency + 20;
}