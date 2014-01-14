
package com.dongle.SystemTest1;

import org.chronotext.cinder.CinderDelegate;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("SystemTest1");
  }

  CinderDelegate delegate;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    delegate = new CinderDelegate(this, null);
    setContentView(delegate.getView());
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    delegate.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    delegate.onResume();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();
    delegate.onDestroy();
  }
}
