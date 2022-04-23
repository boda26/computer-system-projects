from flask import Flask, render_template, request, jsonify
import os
import requests
from datetime import datetime, timedelta, time
import re

app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/weather" (middleware):
@app.route('/weather', methods=["POST"])
def POST_weather():
  course = request.form["course"]
  course = course.upper()
  course1 = course.replace(" ", "")
  course_tuple = [re.findall(r'(\w+?)(\d+)', course1)[0]]
  subject = course_tuple[0][0]
  number = course_tuple[0][1]
  
  # Ensure `subject` contains the subject (ex: "CS") and `number` contains the course number (ex: 240).
  server_url = os.getenv('COURSES_MICROSERVICE_URL')
  r = requests.get(f'{server_url}/{subject}/{number}/')
  course_gpa_data = r.json()
  
  if 'Start Time' not in course_gpa_data:
    return "This course does not exist!", 404


# Use the result in your code (I'm just printing to console here, but you'll use it for your result):
  #print(course_gpa_data['Days of Week'])
  #print(course_gpa_data['Start Time'])
  #print(course_gpa_data['course'])

  time_now = datetime.now()
  weekday_str = time_now.strftime("%A")

  weekday_num = 0
  if (weekday_str == "Monday"):
    weekday_num = 1
  elif (weekday_str == "Tuesday"):
    weekday_num = 2
  elif (weekday_str == "Wednesday"):
    weekday_num = 3
  elif (weekday_str == "Thursday"):
    weekday_num = 4
  elif (weekday_str == "Friday"):
    weekday_num = 5
  elif (weekday_str == "Saturday"):
    weekday_num = 6
  else:
    weekday_num = 7

  timeslot_list = list(course_gpa_data['Days of Week'])
  for i in range(len(timeslot_list)):
    if (timeslot_list[i] == 'M'):
      timeslot_list[i] = 1
    elif (timeslot_list[i] == 'T'):
      timeslot_list[i] = 2
    elif (timeslot_list[i] == 'W'):
      timeslot_list[i] = 3
    elif (timeslot_list[i] == 'R'):
      timeslot_list[i] = 4
    elif (timeslot_list[i] == 'F'):
      timeslot_list[i] = 5
    elif (timeslot_list[i] == 'S'):
      timeslot_list[i] = 6
    else:
      timeslot_list[i] = 7
  
  coursetime_split_space = course_gpa_data['Start Time'].split(' ')
  coursetime_split_colon = coursetime_split_space[0].split(':')
  coursetime_hour = int(coursetime_split_colon[0])
  if (coursetime_split_space[1] == 'PM'):
    if coursetime_hour != 12:
      coursetime_hour += 12
  coursetime_min = int(coursetime_split_colon[1])

  nextclass_num = timeslot_list[0]
  
  for num in timeslot_list:
    if (weekday_num < num):
      nextclass_num = num
      break
    elif (weekday_num == num):
      time_class = datetime.now()
      time_class = time_class.replace(hour = coursetime_hour, minute = coursetime_min)
      if datetime.now() < time_class:
        nextclass_num = num
        break
  #print(nextclass_num)
  add_days = 0
  if nextclass_num > weekday_num:
    add_days = nextclass_num - weekday_num
  elif nextclass_num == weekday_num:
    add_days = 0
  else:
    add_days = nextclass_num - weekday_num + 7
  
  nextCourseMeetingTime = datetime.now() + timedelta(days = add_days)
  nextCourseMeetingTime = nextCourseMeetingTime.replace(hour = coursetime_hour, minute = coursetime_min, second = 0, microsecond = 0)
  nextCourseYear = nextCourseMeetingTime.strftime("%Y")
  nextCourseMonth = nextCourseMeetingTime.strftime("%m")
  nextCourseDay = nextCourseMeetingTime.strftime("%d")
  nextCourseClock = nextCourseMeetingTime.strftime("%H:%M:%S")
  nextCourseTimeStr = nextCourseYear + '-' + nextCourseMonth + '-' + nextCourseDay + ' ' + nextCourseClock
  
  forecastTime = nextCourseMeetingTime + timedelta(minutes = -30)
  
  forecastYear = forecastTime.strftime("%Y")
  forecastMonth = forecastTime.strftime("%m")
  forecastDay = forecastTime.strftime("%d")
  forecastClock = forecastTime.strftime("%H:%M:%S")
  forecastClock_hour = forecastClock.split(':')[0]
  forecastTimeStr = forecastYear + '-' + forecastMonth + '-' + forecastDay + 'T' + forecastClock + '-05:00'
  formatTime = forecastYear + '-' + forecastMonth + '-' + forecastDay + 'T' + forecastClock_hour + ':00:00-05:00'
  #print(formatTime)

  weather_url = os.getenv('WEATHER_URL')
  rr = requests.get(f'{weather_url}')
  weather_data = rr.json()
  periods_list = weather_data["properties"]["periods"]
  for period in periods_list:
    if period['startTime'] == formatTime:
      temperature = period["temperature"]
      shortForecast = period["shortForecast"]
      return jsonify(
        course = course, 
        nextCourseMeeting = nextCourseTimeStr, 
        forecastTime = forecastTimeStr, 
        temperature = temperature, 
        shortForecast = shortForecast
        )
  return jsonify({"error": "Next class is more than a week from today, be relax!"}), 400


